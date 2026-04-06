package com.czx.controller;

import com.czx.pojo.ClazzDeleteException;
import com.czx.pojo.Result;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.dao.DuplicateKeyException;
import org.springframework.stereotype.Component;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;

@RestControllerAdvice
public class GlobalExceptionController {
    private static final Logger log= LoggerFactory.getLogger(GlobalExceptionController.class);
//    @ExceptionHandler
//    public Result error(Exception e){
//        return Result.error("服务器异常");
//    }

    @ExceptionHandler
    public Result error1(DuplicateKeyException e){
        log.info("数据已存在，请重新输入");
        e.printStackTrace();
        String message=e.getMessage();
        int i=message.indexOf("Duplicate entry");
        String errorMsg=message.substring(i);
        String[] arr=errorMsg.split(" ");
        return Result.error(arr[2]+"已存在");
    }

    @ExceptionHandler
    public Result error2(ClazzDeleteException e){
        log.info("该班级下有学生，不能删除");
        e.printStackTrace();
        return Result.error(e.getMessage());
    }
}
