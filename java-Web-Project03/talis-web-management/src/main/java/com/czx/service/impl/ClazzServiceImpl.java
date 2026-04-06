package com.czx.service.impl;

import com.czx.mapper.ClazzMapper;
import com.czx.pojo.Clazz;
import com.czx.pojo.ClazzDeleteException;
import com.czx.pojo.ClazzQueryParam;
import com.czx.pojo.PageResult;
import com.czx.service.ClazzService;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.List;

@Service
public class ClazzServiceImpl implements ClazzService {

    @Autowired
    private ClazzMapper clazzMapper;

    @Transactional(rollbackFor={Exception.class})
    @Override
    public PageResult<Clazz> page(ClazzQueryParam clazzQueryParam) {
        PageHelper.startPage(clazzQueryParam.getPage(), clazzQueryParam.getPageSize());

        List<Clazz> list=clazzMapper.list(clazzQueryParam);

        LocalDate nowTime=LocalDate.now();
        for(Clazz clazz:list){
            if(nowTime.isBefore(clazz.getBeginDate())) clazz.setStatus("未开始");
            else if(nowTime.isAfter(clazz.getEndDate())) clazz.setStatus("已结束");
            else clazz.setStatus("进行中");
        }

        Page<Clazz> p=(Page<Clazz>)list;
        return new PageResult(p.getResult(),p.getTotal());
    }

    @Override
    public void dalete(Integer id) {
        if(clazzMapper.countStudent(id)>0) throw new ClazzDeleteException("对不起, 该班级下有学生, 不能直接删除");
        clazzMapper.deleteClazz(id);
    }

    @Override
    public void save(Clazz clazz) {
        clazz.setCreateTime(LocalDateTime.now());
        clazz.setUpdateTime(LocalDateTime.now());
        clazzMapper.insertClazz(clazz);
    }

    @Override
    public Clazz getById(Integer id) {
        return clazzMapper.getById(id);
    }

    @Override
    public void update(Clazz clazz) {
        clazzMapper.updateClazz(clazz);
    }

    @Override
    public List<Clazz> findAll() {
        return clazzMapper.findAll();
    }
}
