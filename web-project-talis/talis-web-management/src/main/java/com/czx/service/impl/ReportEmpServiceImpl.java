package com.czx.service.impl;

import com.czx.mapper.EmpMapper;
import com.czx.pojo.JobOption;
import com.czx.service.ReportEmpService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Map;

@Service
public class ReportEmpServiceImpl implements ReportEmpService {

    @Autowired
    private EmpMapper empMapper;
    @Override
    public JobOption getEmpJobData() {
        List<Map<String, Object>> list = empMapper.countEmpJobData();
        List<Object> jobList = list.stream().map(datamap -> datamap.get("position")).toList();
        List<Object> dataList = list.stream().map(datamap ->datamap.get("number")).toList();

        return new JobOption(jobList,dataList);
    }

    @Override
    public List<Map<String, Integer>> getEmpGenderData() {
        return empMapper.countEmpGenderData();
    }
}
