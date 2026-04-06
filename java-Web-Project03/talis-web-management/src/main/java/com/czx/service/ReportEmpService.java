package com.czx.service;

import com.czx.pojo.JobOption;

import java.util.List;
import java.util.Map;

public interface ReportEmpService {
    JobOption getEmpJobData();

    List<Map<String, Integer>> getEmpGenderData();
}
