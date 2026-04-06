package com.czx.service.impl;

import com.czx.mapper.StudentMapper;
import com.czx.pojo.ClazzNumber;
import com.czx.service.ReportStudentService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

@Service
public class ReportStudentServiceImpl implements ReportStudentService {

    @Autowired
    private StudentMapper studentMapper;

    @Transactional(rollbackFor={Exception.class})
    @Override
    public ClazzNumber getStudentCountData() {
        List<Map<String,Object>> list= studentMapper.countStudentData();
        List<Object> clazzList=list.stream().map(datamap-> datamap.get("clazzName")).toList();
        List<Object> dataList=list.stream().map(datamap->datamap.get("studentNumber")).toList();
        return new ClazzNumber(clazzList, dataList);
    }

    @Override
    public List<Map<String, Integer>> getStudentDegreeData() {
        return studentMapper.countStudentDegreeData();
    }
}
