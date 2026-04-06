package com.czx.controller;

import com.czx.anno.Log;
import com.czx.pojo.Dept;
import com.czx.pojo.Result;
import com.czx.service.DeptService;
import jakarta.servlet.http.HttpServletRequest;
import lombok.extern.slf4j.Slf4j;
import org.apache.ibatis.annotations.Update;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

//@Slf4j
@CrossOrigin(origins = "http://localhost:5173")
@RequestMapping("/depts")
@RestController
public class DepController {

    private static final Logger log= LoggerFactory.getLogger(DepController.class);

    @Autowired
    private DeptService deptService;

    @GetMapping
    public Result list(){
        log.info("查询所有部门信息");
        List<Dept> deptlist= deptService.findAll();
        return Result.success(deptlist);
    }
    @Log
    @DeleteMapping
    public Result delete(Integer id){
        log.info("根据ID删除部门：{}", id);
        deptService.deleteById(id);
        return Result.success();
    }

    @Log
    @PostMapping
    public Result add(@RequestBody Dept dept){
        log.info("添加部门：{}", dept);
        deptService.add(dept);
        return Result.success();
    }

    @GetMapping("/{id}")
    public Result getInfo(@PathVariable Integer id){
        log.info("查询部门ID：{}", id);
        Dept dept=deptService.getInfo(id);
        return Result.success(dept);
    }

    @Log
    @PutMapping
    public Result update(@RequestBody Dept dept){
        log.info("修改部门：{}", dept);
        deptService.update(dept);
        return Result.success();
    }
}
