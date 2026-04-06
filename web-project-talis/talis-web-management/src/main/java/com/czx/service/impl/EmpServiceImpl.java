package com.czx.service.impl;

import com.czx.controller.utils.JwtUtils;
import com.czx.mapper.EmpExprMapper;
import com.czx.mapper.EmpMapper;
import com.czx.pojo.*;
import com.czx.service.EmpService;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.CollectionUtils;

import java.time.LocalDateTime;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Service
public class EmpServiceImpl implements EmpService {

    @Autowired
    private EmpMapper empMapper;

    @Autowired
    private EmpExprMapper empExprMapper;

//    ------------------------------------原始分页查询操作--------------------------------------------
//    @Override
//    public PageResult<Emp> page(Integer page, Integer pageSize) {
//        Integer start = (page - 1) * pageSize;
//        List<Emp> rows = empMapper.list(start, pageSize);
//        long total = empMapper.count();
//        return new PageResult<Emp>(rows, total);
//    }

    @Override
    public PageResult<Emp> page(EmpQueryParam empQueryParam) {
        PageHelper.startPage(empQueryParam.getPage(), empQueryParam.getPageSize());

        List<Emp> list = empMapper.list(empQueryParam);

        Page<Emp> p=(Page<Emp>)list;
        return new PageResult<Emp>(p.getResult(),p.getTotal());
    }

    @Transactional(rollbackFor = Exception.class)//事务
    @Override
    public void save(Emp emp) {
        emp.setCreateTime(LocalDateTime.now());
        emp.setUpdateTime(LocalDateTime.now());

        List<EmpExpr> exprList=emp.getExprList();
        empMapper.insert(emp);

        if(!CollectionUtils.isEmpty(exprList)) {
            exprList.forEach(expr->expr.setEmpId(emp.getId()));
            empExprMapper.insertBatch(exprList);
        }
    }

    @Transactional(rollbackFor={Exception.class})
    @Override
    public void delete(List<Integer> ids) {
        empMapper.deleteEmp(ids);
        empExprMapper.deleteExpr(ids);
    }

    @Override
    public Emp getByID(Integer id) {
        return empMapper.getByID(id);
    }

    @Override
    public void update(Emp emp) {
        emp.setUpdateTime(LocalDateTime.now());
        empMapper.updateByID(emp);

        empExprMapper.deleteExpr(Arrays.asList(emp.getId()));
        List<EmpExpr> list=emp.getExprList();
        if(!CollectionUtils.isEmpty(list))list.forEach(expr->expr.setEmpId(emp.getId()));
        empExprMapper.insertBatch(list);
    }

    @Override
    public List<Emp> list() {
        return empMapper.findAll();
    }

    @Override
    public LoginInfo login(Emp emp) {
        Emp e=empMapper.getUserByNameAndPassword(emp);
//        生成 token
        if(e!=null) {
            Map<String, Object> claims = new HashMap<>();
            claims.put("id", e.getId());
            claims.put("username", e.getUsername());
            String token = JwtUtils.generateToken(claims);
            return new LoginInfo(e.getId(), e.getUsername(), e.getName(), token);
        }
        return null;
    }


}
