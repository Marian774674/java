package com.hmdp.service.impl;

import cn.hutool.core.bean.BeanUtil;
import cn.hutool.core.bean.copier.CopyOptions;
import cn.hutool.core.lang.Editor;
import cn.hutool.core.lang.UUID;
import cn.hutool.core.util.RandomUtil;
import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.hmdp.dto.LoginFormDTO;
import com.hmdp.dto.Result;
import com.hmdp.dto.UserDTO;
import com.hmdp.entity.User;
import com.hmdp.mapper.UserMapper;
import com.hmdp.service.IUserService;
import com.hmdp.utils.RegexUtils;
import com.hmdp.utils.SystemConstants;
import com.hmdp.utils.UserHolder;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.connection.BitFieldSubCommands;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.stereotype.Service;

import javax.servlet.http.HttpSession;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeUnit;
@Service
@Slf4j
public class UserServiceImpl extends ServiceImpl<UserMapper, User> implements IUserService {

    @Autowired
    private StringRedisTemplate stringRedisTemplate;

    @Override
    public Result login(LoginFormDTO loginForm, HttpSession session) {
        //校验手机号
        if(RegexUtils.isPhoneInvalid(loginForm.getPhone())){
            return Result.fail("手机号格式错误");
        }
        //校验验证码
//        Object cacheCode = session.getAttribute("code");
        String cacheCode = stringRedisTemplate.opsForValue().get("login:code:" + loginForm.getPhone());
        if(cacheCode == null || !cacheCode.equals(loginForm.getCode())){
            return Result.fail("验证码错误");
        }
        //判断用户是否存在
        User user = query().eq("phone", loginForm.getPhone()).one();
        if(user == null){
            user = createUserWithPhone(loginForm.getPhone());
        }
        //保存用户信息
//        session.setAttribute("user", BeanUtil.copyProperties(user, UserDTO.class));
            //生成token
        String token = UUID.randomUUID().toString(true);
            //存入redis
        Map<String, Object> userMap = BeanUtil.beanToMap(user,new HashMap<>(),
                CopyOptions.create()
                        .setIgnoreNullValue(true)
                        .setFieldValueEditor((fieldName, fieldValue)->fieldValue.toString())
        );

        stringRedisTemplate.opsForHash().putAll("login:token:"+token,userMap);
            //设置有效期
        stringRedisTemplate.expire("login:token:"+token,30,TimeUnit.MINUTES);
            //返回token
        return Result.ok(token);
    }

    private User createUserWithPhone(String phone) {
        User user = new User();
        user.setPhone(phone);
        user.setNickName(SystemConstants.USER_NICK_NAME_PREFIX + RandomUtil.randomString(10));
        save(user);
        return user;
    }

    @Override
    public Result sign() {
        Long userId = UserHolder.getUser().getId();
        LocalDateTime now = LocalDateTime.now();
        String keySuffix = now.format(DateTimeFormatter.ofPattern("yyyyMM"));
        String key = "user:sign:"+userId+keySuffix;
        int dayOfMonth = now.getDayOfMonth();
        stringRedisTemplate.opsForValue().setBit(key, dayOfMonth-1,true);
        return Result.ok();
    }

    @Override
    public Result signCount() {
        Long userId = UserHolder.getUser().getId();
        LocalDateTime now = LocalDateTime.now();
        String keySuffix = now.format(DateTimeFormatter.ofPattern("yyyyMM"));
        String key = "user:sign:"+userId+keySuffix;
        int dayOfMonth = now.getDayOfMonth();
        List<Long> result = stringRedisTemplate.opsForValue().bitField(
                key, BitFieldSubCommands.create()
                        .get(BitFieldSubCommands.BitFieldType.unsigned(dayOfMonth))
                        .valueAt(0)
        );
        if(result == null || result.isEmpty()){
            return Result.ok(0);
        }
        Long num = result.get(0);
        if(num == null || num == 0){
            return Result.ok(0);
        }
        int cnt = 0;
        while(true){
            if((num & 1) == 0){
                break;
            }else{
                cnt++;
            }
            num >>>= 1;
        }
        return Result.ok(cnt);
    }

    @Override
    public Result send(String phone, HttpSession session) {
        //判断手机号是否合法
        if(RegexUtils.isPhoneInvalid(phone)){
            return Result.fail("手机号格式错误");
        }
//        生成验证码
        String code = RandomUtil.randomNumbers(6);
        //保存验证码
//        session.setAttribute("code", code);
        stringRedisTemplate.opsForValue()
                .set("login:code:" + phone,code,5,TimeUnit.MINUTES);
        //发送验证码
        log.info("发送验证码成功，验证码：{}", code);
        return Result.ok();
    }
}
