package com.czx.controller;


import com.czx.pojo.LogQueryParam;
import com.czx.pojo.OperateLog;
import com.czx.pojo.PageResult;
import com.czx.pojo.Result;
import com.czx.service.LogService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;


@RestController
public class LogController{

    private static final Logger log= LoggerFactory.getLogger(LogController.class);

    @Autowired
    private LogService logService;

    @GetMapping("/log/page")
    public Result page(LogQueryParam logQueryParam){
        log.info("分页查询：{}",logQueryParam);
        PageResult<OperateLog> pageResult=logService.page(logQueryParam);
        return Result.success(pageResult);
    }
}
