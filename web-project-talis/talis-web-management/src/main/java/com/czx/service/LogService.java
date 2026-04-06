package com.czx.service;

import com.czx.pojo.LogQueryParam;
import com.czx.pojo.OperateLog;
import com.czx.pojo.PageResult;

public interface LogService {
    PageResult<OperateLog> page(LogQueryParam logQueryParam);
}
