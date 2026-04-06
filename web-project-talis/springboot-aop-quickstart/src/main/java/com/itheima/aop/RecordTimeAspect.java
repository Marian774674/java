package com.itheima.aop;


import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

@Aspect
@Component
public class RecordTimeAspect {

    private static final Logger log= LoggerFactory.getLogger(RecordTimeAspect.class);

    @Around("execution(* com.itheima.controller.*.*(..))")
    public Object recordTime(ProceedingJoinPoint pjp) throws Throwable {
        long begin = System.currentTimeMillis();

        Object result = pjp.proceed();

        long end = System.currentTimeMillis();
        log.info("{}耗时：{}ms",pjp.getSignature(),end-begin);
        return result;
    }
}
