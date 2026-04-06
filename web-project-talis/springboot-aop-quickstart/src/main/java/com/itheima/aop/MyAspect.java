package com.itheima.aop;


import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

@Component
@Aspect
public class MyAspect {

    @Pointcut("execution(* com.itheima.service.impl.*.*(..))")
    public void pt(){}

    private static final Logger log= LoggerFactory.getLogger(RecordTimeAspect.class);

    //    认识五种通知类型:
    @Before("pt()")
    public void before(JoinPoint joinPoint){
        log.info("before方法执行前调用...");
//        获取目标对象
        Object target = joinPoint.getTarget();
        log.info("方法对象：{}", target);
//        获取目标类
        String className = joinPoint.getTarget().getClass().getName();
        log.info("类名：{}", className);
//        获取目标方法名
        String methodName = joinPoint.getSignature().getName();
        log.info("方法名：{}", methodName);
//        获取目标方法参数
        Object[] args = joinPoint.getArgs();
        log.info("参数：{}", args);
    }

    @Around("pt()")
    public Object around(ProceedingJoinPoint pjp) throws Throwable {
        log.info("Around方法执行前调用");
        Object ret = pjp.proceed();
        //        获取目标对象
        Object target = pjp.getTarget();
        log.info("方法对象：{}", target);
//        获取目标类
        String className = pjp.getTarget().getClass().getName();
        log.info("类名：{}", className);
//        获取目标方法名
        String methodName = pjp.getSignature().getName();
        log.info("方法名：{}", methodName);
//        获取目标方法参数
        Object[] args = pjp.getArgs();
        log.info("参数：{}", args);
        log.info("Around方法执行后调用");
        return ret;
    }

    @After("pt()")
    public void after(){
        log.info("After方法执行后调用");
    }

    @AfterReturning("pt()")
    public void afterReturning(){
        log.info("AfterReturning方法返回后调用");
    }

    @AfterThrowing("pt()")
    public void afterThrowing(){
        log.info("AfterThrowing方法出异常后调用");
    }
}
