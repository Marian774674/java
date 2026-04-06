package com.czx.controller;


import com.czx.anno.Log;
import com.czx.pojo.Emp;
import com.czx.pojo.EmpQueryParam;
import com.czx.pojo.PageResult;
import com.czx.pojo.Result;
import com.czx.service.EmpService;
import lombok.extern.flogger.Flogger;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.List;

@CrossOrigin(origins = "http://localhost:5173")
@RequestMapping("/emps")
@RestController
public class EmpController {
//    private static final Logger log= LoggerFactory.getLogger(DepController.class);
    private static final Logger log=LoggerFactory.getLogger(EmpController.class);

    @Autowired
    private EmpService empService;
    @GetMapping
    public Result page(EmpQueryParam empQueryParam){
        log.info("分页查询{}",empQueryParam);
        PageResult<Emp> pageResult= empService.page(empQueryParam);
        return Result.success(pageResult);
    }

    @Log
    @PostMapping
    public Result save(@RequestBody Emp emp){
        log.info("添加员工：{}",emp);
        empService.save(emp);
        return Result.success();
    }

    @Log
    @DeleteMapping
    public Result delete(@RequestParam List<Integer> ids){
        log.info("删除员工：{}",ids);
        empService.delete(ids);
        return Result.success();
    }

    @GetMapping("/{id}")
    public Result getInfo(@PathVariable Integer id){
        log.info("查询员工：{}",id);
        Emp emp=empService.getByID(id);
        return Result.success(emp);
    }

    @Log
    @PutMapping
    public Result update(@RequestBody Emp emp){
        log.info("修改员工：{}",emp);
        empService.update(emp);
        return Result.success();
    }

    @GetMapping("/list")
    public Result list(){
        log.info("查询所有员工");
        List<Emp> list=empService.list();
        return Result.success(list);
    }
}
