package com.czx.controller;


import com.czx.pojo.Clazz;
import com.czx.pojo.ClazzQueryParam;
import com.czx.pojo.PageResult;
import com.czx.pojo.Result;
import com.czx.service.ClazzService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RequestMapping("/clazzs")
@RestController
public class ClazzController {

    private static final Logger log= LoggerFactory.getLogger(ClazzController.class);

    @Autowired
    private ClazzService clazzService;

    @GetMapping
    public Result list(ClazzQueryParam clazzQueryParam){
        log.info("分页查询：{}",clazzQueryParam);
        PageResult<Clazz> pageResult=clazzService.page(clazzQueryParam);
        return Result.success(pageResult);
    }

    @DeleteMapping("/{id}")
    public Result delete(@PathVariable Integer id){
        log.info("删除班级：{}",id);
        clazzService.dalete(id);
        return Result.success();
    }

    @PostMapping
    public Result save(@RequestBody Clazz clazz){
        log.info("新增班级：{}",clazz.getName());
        clazzService.save(clazz);
        return Result.success();
    }

    @GetMapping("/{id}")
    public Result getInfo(@PathVariable Integer id){
        log.info("查询班级：{}",id);
        Clazz clazz=clazzService.getById(id);
        return Result.success(clazz);
    }

    @PutMapping
    public Result update(@RequestBody Clazz clazz){
        log.info("修改班级：{}",clazz);
        clazzService.update(clazz);
        return Result.success();
    }

    @GetMapping("/list")
    public Result list(){
        log.info("查询所有班级");
        List<Clazz> list=clazzService.findAll();
        return Result.success(list);
    }
}
