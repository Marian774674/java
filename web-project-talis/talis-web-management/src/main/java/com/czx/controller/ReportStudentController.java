package com.czx.controller;


import com.czx.pojo.ClazzNumber;
import com.czx.pojo.Result;
import com.czx.service.ReportStudentService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;
import java.util.Map;

@RequestMapping("/report")
@RestController
public class ReportStudentController {

    private static final Logger log= LoggerFactory.getLogger(ReportStudentController.class);

    @Autowired
    private ReportStudentService reportStudentService;

    @GetMapping("/studentCountData")
    public Result getStudentCountData(){
        log.info("统计各班级人数");
        ClazzNumber clazzNumber= reportStudentService.getStudentCountData();
        return Result.success(clazzNumber);
    }

    @GetMapping("/studentDegreeData")
    public Result getStudentDegreeData(){
        log.info("统计各班级学历人数");
        List<Map<String,Integer>>degreeList= reportStudentService.getStudentDegreeData();
        return Result.success(degreeList);
    }
}
