package com.hmdp.service.impl;

import cn.hutool.core.bean.BeanUtil;
import cn.hutool.core.util.BooleanUtil;
import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.hmdp.dto.Result;
import com.hmdp.dto.ScrollResult;
import com.hmdp.dto.UserDTO;
import com.hmdp.entity.Blog;
import com.hmdp.entity.Follow;
import com.hmdp.entity.User;
import com.hmdp.mapper.BlogMapper;
import com.hmdp.service.IBlogService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.hmdp.service.IFollowService;
import com.hmdp.service.IUserService;
import com.hmdp.utils.SystemConstants;
import com.hmdp.utils.UserHolder;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.data.redis.core.ZSetOperations;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

/**
 * <p>
 * 服务实现类
 * </p>
 *
 * @author 虎哥
 * @since 2021-12-22
 */
@Service
public class BlogServiceImpl extends ServiceImpl<BlogMapper, Blog> implements IBlogService {

    @Resource
    private IUserService userService;
    @Resource
    private StringRedisTemplate stringRedisTemplate;
    @Resource
    private IFollowService followService;

    /**
     * 查询热门博客
     * @param current
     * @return
     */
    @Override
    public Result queryBlogHot(Integer current) {
        // 根据用户查询
        Page<Blog> page = query()
                .orderByDesc("liked")
                .page(new Page<>(current, SystemConstants.MAX_PAGE_SIZE));
        // 获取当前页数据
        List<Blog> records = page.getRecords();

        // 查询用户
        records.forEach(blog -> {
            this.queryBlogUser(blog);
            this.isBlogLiked(blog);
        });

        return Result.ok(records);
    }

    /**
     * 查询博客详情
     * @param id
     * @return
     */
    @Override
    public Result queryBlogById(Long id) {
        Blog blog = getById(id);
        if (blog == null) {
            return Result.fail("数据不存在");
        }

        queryBlogUser(blog);
        isBlogLiked(blog);
        return Result.ok(blog);
    }

    /**
     * 判断当前用户是否点赞了该博客
     * @param blog
     */
    private void isBlogLiked(Blog blog) {
        UserDTO user = UserHolder.getUser();
        if (user == null){
            return;
        }
        Long userId = user.getId();
        String key = "blog:liked:" + blog.getId();
        Double score = stringRedisTemplate.opsForZSet().score(key, userId.toString());
        blog.setIsLike(score != null);
    }

    /**
     * 点赞博客
     * @param id
     * @return
     */
    @Override
    public Result likeBlog(Long id) {
        String userId = UserHolder.getUser().getId().toString();
        String key = "blog:liked:" + id;
        // 判断是否已经点赞
        Double score = stringRedisTemplate.opsForZSet().score(key, userId);
        if (score == null) {
            // 保存数据
            boolean isSuccess = update().setSql("liked = liked + 1").eq("id", id).update();
            if (isSuccess) {
                stringRedisTemplate.opsForZSet().add(key, userId,System.currentTimeMillis());
            }
        } else {
            // 移除数据
            boolean isSuccess = update().setSql("liked = liked - 1").eq("id", id).update();
            if (isSuccess) {
                stringRedisTemplate.opsForZSet().remove(key, userId);
            }
        }
        return Result.ok();
    }

    /**
     * 保存博客
     * @param blog
     * @return
     */
    @Override
    public Result saveBlog(Blog blog) {
        // 获取登录用户
        UserDTO user = UserHolder.getUser();
        blog.setUserId(user.getId());
        // 保存探店博文
        save(blog);
        // 查询收件箱
        List<Follow> follows = followService.query().eq("follow_id", user.getId()).list();
        // 推送数据
        for (Follow follow : follows) {
            Long followId = follow.getId();
            String key = "feed:" + followId;
            stringRedisTemplate.opsForZSet().add(key, blog.getId().toString(), System.currentTimeMillis());
        }
        // 返回id
        return Result.ok(blog.getId());
    }

    /**
     * 查询用户关注人的最新博客
     * @param max
     * @param offset
     * @return
     */
    @Override
    public Result queryBlogOfFollow(Long max, Integer offset) {
        // 1. 获取当前登录用户ID
        Long userId = UserHolder.getUser().getId();

        // 2. 查询收件箱（Feed流）：基于ZSet的滚动分页查询
        // key: feed:{userId}, min: 0, max: max(上一次的最小时间戳), offset: 偏移量, count: 2(每次查2条)
        Set<ZSetOperations.TypedTuple<String>> typedTuples = stringRedisTemplate.opsForZSet()
                .reverseRangeByScoreWithScores("feed:" + userId, 0, max, offset, 2);

        // 3. 判空：如果没有数据，直接返回空结果
        if (typedTuples == null || typedTuples.isEmpty()) {
            return Result.ok();
        }

        // 4. 解析数据，提取博客ID列表，并计算下一次查询的minTime和offset
        List<Long> ids = new ArrayList<>();
        long minTime = 0; // 记录本次查询中最小的时间戳
        int os = 1;       // 记录最小时间戳出现的次数（偏移量）

        for (ZSetOperations.TypedTuple<String> tuple : typedTuples) {
            // 获取博客ID
            ids.add(Long.valueOf(tuple.getValue()));
            // 获取时间戳（分数）
            long time = tuple.getScore().longValue();

            // 计算minTime和os
            if (minTime == time) {
                // 如果当前时间戳等于minTime，说明是重复的最小值，偏移量+1
                os++;
            } else {
                // 如果当前时间戳小于minTime（因为是倒序，后面的更小），更新minTime，重置os为1
                minTime = time;
                os = 1;
            }
        }

        // 5. 根据ID列表查询数据库中的博客详情
        // 使用StrUtil.join将ID列表转换为逗号分隔的字符串
        String idStr = StrUtil.join(",", ids);
        // 使用MySQL的field函数保持查询结果与ID列表的顺序一致
        List<Blog> blogs = query()
                .in("id", ids)
                .last("ORDER BY FIELD(id," + idStr + ")")
                .list();

        // 6. 补充博客的用户信息和点赞状态
        for (Blog blog : blogs) {
            queryBlogUser(blog);   // 查询发布博客的用户信息
            isBlogLiked(blog);     // 判断当前用户是否点赞
        }

        // 7. 封装滚动分页结果
        ScrollResult scrollResult = new ScrollResult();
        scrollResult.setList(blogs); // 博客列表
        scrollResult.setMinTime(minTime); // 最小时间戳，用于下次查询的max参数
        scrollResult.setOffset(os);       // 偏移量，用于下次查询的offset参数

        return Result.ok(scrollResult);
    }

    /**
     * 查询点赞用户
     * @param id
     * @return
     */
    @Override
    public Result queryBlogLikes(Long id) {
        String key = "blog:liked:" + id;

        Set<String> top5 = stringRedisTemplate.opsForZSet().reverseRange(key, 0, 4);

        if(top5 == null || top5.isEmpty()){
            return Result.ok();
        }

        List<Long> ids = top5.stream().map(Long::valueOf).collect(Collectors.toList());
        String idStr = StrUtil.join(",", ids);

        List<UserDTO> userDTOS = userService.query()
                .in("id",ids).last("order by field(id,"+ idStr +")").list()
                .stream()
                .map(user -> BeanUtil.copyProperties(user, UserDTO.class))
                .collect(Collectors.toList());

        return Result.ok(userDTOS);
    }

    /**
     * 查询用户信息
     * @param blog
     */
    private void queryBlogUser(Blog blog) {
        Long userId = blog.getUserId();
        User user = userService.getById(userId);
        blog.setName(user.getNickName());
        blog.setIcon(user.getIcon());
    }
}
