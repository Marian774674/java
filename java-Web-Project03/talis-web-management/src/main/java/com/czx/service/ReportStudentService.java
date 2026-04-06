package com.czx.service;

import com.czx.pojo.ClazzNumber;

import java.util.List;
import java.util.Map;

public interface ReportStudentService {

    ClazzNumber getStudentCountData();

    List<Map<String, Integer>> getStudentDegreeData();
}
