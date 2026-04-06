package com.czx.controller;

import com.czx.anno.Log;
import com.czx.pojo.PageResult;
import com.czx.pojo.Result;
import com.czx.pojo.Student;
import com.czx.pojo.StudentQueryParam;
import com.czx.service.StudentService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RequestMapping("/students")
@RestController
public class StudentController {

    private static final Logger log = LoggerFactory.getLogger(StudentController.class);

    @Autowired
    private StudentService studentService;

    @GetMapping
    public Result page(StudentQueryParam studentQueryParam){
        log.info("分页查询{}",studentQueryParam);
        PageResult<Student> pageResult = studentService.page(studentQueryParam);
        return Result.success(pageResult);
    }

    @Log
    @PostMapping
    public Result save(@RequestBody Student student){
        log.info("添加学员：{}", student);
        studentService.save(student);
        return Result.success();
    }

    @GetMapping("/{id}")
    public Result getById(@PathVariable Integer id){
        log.info("查询学员：{}",id);
        Student student=studentService.getById(id);
        return Result.success(student);
    }

    @Log
    @PutMapping
    public Result update(@RequestBody Student student){
        log.info("更新学生信息：{}", student.getName());
        studentService.update(student);
        return Result.success();
    }

    @Log
    @DeleteMapping("/{ids}")
    public Result delete(@PathVariable List<Integer> ids) {
        log.info("删除学生：{}", ids);
        studentService.delete(ids);
        return Result.success();
    }

    @Log
    @PutMapping("/violation/{id}/{score}")
    public Result updateViolation(@PathVariable Integer id,@PathVariable Integer score){
        log.info("更新学生违规信息：{}{}", id,score);
        studentService.updateViolation(id,score);
        return Result.success();
    }
}
