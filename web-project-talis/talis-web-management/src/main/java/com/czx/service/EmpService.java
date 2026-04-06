package com.czx.service;

import com.czx.pojo.Emp;
import com.czx.pojo.EmpQueryParam;
import com.czx.pojo.LoginInfo;
import com.czx.pojo.PageResult;

import java.util.List;

public interface EmpService {


    PageResult<Emp> page(EmpQueryParam empQueryParam);

    void save(Emp emp);

    void delete(List<Integer> list);

    Emp getByID(Integer id);

    void update(Emp emp);

    List<Emp> list();

    LoginInfo login(Emp emp);
}
