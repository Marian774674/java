package com.czx.service;

import com.czx.pojo.Clazz;
import com.czx.pojo.ClazzQueryParam;
import com.czx.pojo.PageResult;

import java.util.List;

public interface ClazzService {
    PageResult<Clazz> page(ClazzQueryParam clazzQueryParam);

    void dalete(Integer id);

    void save(Clazz clazz);

    Clazz getById(Integer id);

    void update(Clazz clazz);

    List<Clazz> findAll();
}
