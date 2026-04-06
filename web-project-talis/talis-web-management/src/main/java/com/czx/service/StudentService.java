package com.czx.service;

import com.czx.pojo.PageResult;
import com.czx.pojo.Student;
import com.czx.pojo.StudentQueryParam;

import java.util.List;

public interface StudentService {
    PageResult<Student> page(StudentQueryParam studentQueryParam);

    void save(Student student);

    Student getById(Integer id);

    void update(Student student);

    void delete(List<Integer> ids);

    void updateViolation(Integer id, Integer score);
}
