package com.hmdp.service.impl;

import cn.hutool.core.util.BooleanUtil;
import cn.hutool.core.util.StrUtil;
import cn.hutool.json.JSONObject;
import cn.hutool.json.JSONUtil;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.hmdp.dto.Result;
import com.hmdp.entity.Shop;
import com.hmdp.mapper.ShopMapper;
import com.hmdp.service.IShopService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.hmdp.utils.RedisData;
import org.springframework.data.geo.Distance;
import org.springframework.data.geo.GeoResult;
import org.springframework.data.geo.GeoResults;
import org.springframework.data.redis.connection.RedisGeoCommands;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.data.redis.domain.geo.GeoReference;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.annotation.Resource;
import java.time.LocalDateTime;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

@Service
public class ShopServiceImpl extends ServiceImpl<ShopMapper, Shop> implements IShopService {

    @Resource
    private StringRedisTemplate stringRedisTemplate;

    private static final ExecutorService CACHE_REBUILD_EXECUTOR = Executors.newFixedThreadPool(10);

    /**
     * 更新店铺信息
     * @param shop
     * @return
     */
    @Override
    @Transactional
    public Result update(Shop shop) {
        if(shop.getId() == null){
            return Result.fail("店铺id不能为空");
        }
        //先操作数据库
        updateById(shop);
        //在删除缓存
        stringRedisTemplate.delete("cache:shop:" + shop.getId());
        return Result.ok();
    }

    /**
     * 根据id查询店铺信息
     * @param id
     * @return
     */
    @Override
    public Result queryById(Long id) {
        //解决缓存穿透，基于互斥锁解决缓存击穿
        Shop shop = queryByIdWithMutex(id);

        //基于逻辑外键解决缓存击穿
//        Shop shop = queryByIdWithLogicExpire(id);

        if(shop == null){
            return Result.fail("店铺不存在");
        }
        return Result.ok(shop);
    }

    /**
     * 逻辑外键解决缓存击穿
     * @param id
     * @return
     */
    private Shop queryByIdWithLogicExpire(Long id) {
        //先查缓存
        String key = "cache:shop:" + id;
        String shopJson = stringRedisTemplate.opsForValue().get(key);
        //缓存未命中
        if(StrUtil.isBlank(shopJson)){
            return null;
        }
        //缓存命中，判断是否过期
        //反序列化为对象
        RedisData redisData = JSONUtil.toBean(shopJson, RedisData.class);
        Shop shop = JSONUtil.toBean((JSONObject) redisData.getData(), Shop.class);
        //未过期，直接返回
        if(redisData.getExpireTime().isAfter(LocalDateTime.now())){
            return shop;
        }
        //已过期，需要缓存重建
        //缓存重建，获取锁
        String lockKey = "lock:shop:" + id;
        boolean isLock = tryLock(lockKey);
        //获取锁成功，创建独立线程开始重建缓存，然后返回过期数据
        if(isLock){
            CACHE_REBUILD_EXECUTOR.submit(() -> {
                try {
                    //缓存重建
                    this.saveShop2Redis(id, 30L);
                } catch (Exception e) {
                    e.printStackTrace();
                } finally {
                    //释放锁
                    unLock(lockKey);
                }
            });
        }
        //获取锁失败，返回旧数据
        stringRedisTemplate.opsForValue().set(key,JSONUtil.toJsonStr(shop),30,TimeUnit.MINUTES);
        return shop;
    }

    /**
     * 互斥锁解决缓存击穿
     * @param id
     */
    private Shop queryByIdWithMutex(Long id){
        //先查缓存
        String key = "cache:shop:" + id;
        String shopJson = stringRedisTemplate.opsForValue().get(key);
        if(StrUtil.isNotBlank(shopJson)){
            //1.缓存存在，直接返回
            return JSONUtil.toBean(shopJson, Shop.class);
        }
        //判断是否为空值
        if(shopJson != null){//shopJson不是null就是“”
            return null;
        }
        Shop shop = null;
        String lockKey = "lock:shop:" + id;
        try {
            //不存在  获取互斥锁，实现缓存重建
            boolean isLocked = tryLock(lockKey);
            if(!isLocked){
                //获取锁失败
                Thread.sleep(50);
                queryById(id);
            }
            //查询数据库并放入缓存
            shop = getById(id);
            if(shop == null){
                stringRedisTemplate.opsForValue().set(key,"",30,TimeUnit.MINUTES);
                return null;
            }
            stringRedisTemplate.opsForValue().set(key,JSONUtil.toJsonStr(shop),30,TimeUnit.MINUTES);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            //释放锁
            unLock(lockKey);
        }
        return shop;
    }

    //获取锁
    public boolean tryLock(String key){
        Boolean flag = stringRedisTemplate.opsForValue().setIfAbsent(key, "1", 10, TimeUnit.SECONDS);
        return BooleanUtil.isTrue(flag);
    }

    //释放锁
    public void unLock(String key){
        stringRedisTemplate.delete(key);
    }

    /**
     * 根据商铺类型分页查询商铺信息
     * @param typeId
     * @param current
     * @return
     */
    @Override
    public Result queryShopByType(Integer typeId, Integer current, Double x, Double y) {
        // 未提供地理位置坐标，使用数据库分页查询
        if(x == null || y == null){
            Page<Shop> page = query().eq("type_id", typeId)
                    .page(new Page<>(current, 5));
            return Result.ok(page.getRecords());
        }
        // 计算分页起止位置
        int from = (current - 1) * 5;
        int end = current * 5;
        // 构建Redis GEO查询键
        String key = "shop:GEO:" + typeId;
        // 基于Redis GEO搜索指定范围内的店铺，按距离排序并返回距离信息
        GeoResults<RedisGeoCommands.GeoLocation<String>> results = stringRedisTemplate.opsForGeo()
                .search(key, GeoReference.fromCoordinate(x, y),
                        new Distance(5000),
                        RedisGeoCommands.GeoSearchCommandArgs.newGeoSearchArgs().includeDistance().limit(end)
                );
        // GEO查询无结果，返回空列表
        if(results == null){
            return Result.ok(Collections.emptyList());
        }
        List<GeoResult<RedisGeoCommands.GeoLocation<String>>> content = results.getContent();
        // 查询结果不足当前页起始位置，返回空列表
        if(content.size() <= from){
            return Result.ok(Collections.emptyList());
        }
        // 提取店铺ID集合和距离映射关系
        List<Long> ids = new ArrayList<>();
        Map<String, Distance> distanceMap = new HashMap<>();
        content.stream().skip(from).forEach(result -> {
            String name = result.getContent().getName();
            ids.add(Long.valueOf(name));
            Distance distance = result.getDistance();
            distanceMap.put(name,distance);
        });
        // 根据ID批量查询店铺信息，并保持与ID列表相同的顺序
        List<Shop> shops = query().in("id", ids).last("order by field(id," + StrUtil.join(",", ids) + ")").list();
        // 将距离信息设置到店铺对象中
        for (Shop shop : shops) {
            shop.setDistance(distanceMap.get(shop.getId().toString()).getValue());
        }
        return Result.ok(shops);
    }


    //缓存预热，用于单元测试实现添加热点商品数据到Redis
    public void saveShop2Redis(Long id, Long expireSeconds) {
        Shop shop = getById(id);
        RedisData redisData = new RedisData();
        redisData.setData(shop);
        redisData.setExpireTime(LocalDateTime.now().plusSeconds(expireSeconds));
        stringRedisTemplate.opsForValue().set("cache:shop:" + id, JSONUtil.toJsonStr(redisData));
    }
}




























