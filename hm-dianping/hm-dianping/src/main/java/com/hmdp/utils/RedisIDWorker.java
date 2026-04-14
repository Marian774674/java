package com.hmdp.utils;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.stereotype.Component;

import java.time.LocalDateTime;
import java.time.ZoneOffset;
import java.time.format.DateTimeFormatter;

@Component
public class RedisIDWorker {

    @Autowired
    private StringRedisTemplate stringRedisTemplate;

    private final long SHIFT = 32;

    private static final long BEGIN_TIMESTAMP = 1640995200L;

    public long nextId(String keyPrefix){
        //生成时间戳
        LocalDateTime now = LocalDateTime.now();
        long nowEpochSecond = now.toEpochSecond(ZoneOffset.UTC);
        long timestamp = nowEpochSecond - BEGIN_TIMESTAMP;
        //生成序列号
        String date = now.format(DateTimeFormatter.ofPattern("yyyy:MM:dd"));
        long count = stringRedisTemplate.opsForValue().increment("icr:" + keyPrefix + ":" + date);
        //拼接返回
        return timestamp << SHIFT | count;
    }
}
