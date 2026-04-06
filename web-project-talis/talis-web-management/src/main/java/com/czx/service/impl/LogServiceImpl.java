package com.czx.service.impl;

import com.czx.mapper.OperateLogMapper;
import com.czx.pojo.LogQueryParam;
import com.czx.pojo.OperateLog;
import com.czx.pojo.PageResult;
import com.czx.service.LogService;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class LogServiceImpl implements LogService {

    @Autowired
    private OperateLogMapper operateLogMapper;


    @Override
    public PageResult<OperateLog> page(LogQueryParam logQueryParam) {
        PageHelper.startPage(logQueryParam.getPage(),logQueryParam.getPageSize());

        List<OperateLog> list=operateLogMapper.list(logQueryParam);
        Page<OperateLog>p=(Page<OperateLog>)list;
        return new PageResult<OperateLog>(p.getResult(),p.getTotal());
    }
}
