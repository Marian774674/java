package com.czx.controller;


import com.czx.pojo.JobOption;
import com.czx.pojo.Result;
import com.czx.service.ReportEmpService;
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
public class ReportEmpController {

    @Autowired
    private ReportEmpService reportService;

    private static final Logger log= LoggerFactory.getLogger(ReportEmpController.class);

    @GetMapping("/empJobData")
    public Result getEmpJobData(){
        log.info("统计员工职位人数");
        JobOption jobOption =reportService.getEmpJobData();
        return Result.success(jobOption);
    }

    @GetMapping("/empGenderData")
    public Result getEmpGenderData(){
        log.info("统计员工性别人数");
        List<Map<String,Integer>>genderList= reportService.getEmpGenderData();
        return Result.success(genderList);
    }
}
