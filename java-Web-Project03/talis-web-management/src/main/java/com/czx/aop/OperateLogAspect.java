package com.czx.aop;


import com.czx.controller.ClazzController;
import com.czx.utils.CurrentHolder;
import com.czx.mapper.OperateLogMapper;
import com.czx.pojo.OperateLog;
import org.apache.ibatis.annotations.Mapper;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.context.annotation.Configuration;
import org.springframework.stereotype.Component;

import java.time.LocalDateTime;
import java.util.Arrays;

@Aspect
@Component
public class OperateLogAspect {

    private static final Logger log= LoggerFactory.getLogger(ClazzController.class);

    @Autowired
    private OperateLogMapper operateLogMapper;

    @Around("@annotation(com.czx.anno.Log)")
    public Object logOperation(ProceedingJoinPoint pjp) throws Throwable {
        long begin = System.currentTimeMillis();
        Object result = pjp.proceed();
        long end = System.currentTimeMillis();
        long costTime = end - begin;

        String methodName = pjp.getSignature().getName();
        OperateLog olog = new OperateLog();
        olog.setOperateEmpId(CurrentHolder.getCurrentId());
        olog.setOperateTime(LocalDateTime.now());
        olog.setClassName(pjp.getTarget().getClass().getName());
        olog.setMethodName(methodName);
        olog.setMethodParams(Arrays.toString(pjp.getArgs()));
        olog.setReturnValue(result != null ? result.toString() : "void");
        olog.setCostTime(costTime);

        log.info("操作日志：{}",olog);

        operateLogMapper.insert(olog);
        return result;
    }
}
