package com.czx.mapper;


import com.czx.pojo.Student;
import com.czx.pojo.StudentQueryParam;
import org.apache.ibatis.annotations.*;

import java.util.List;
import java.util.Map;

@Mapper
public interface StudentMapper {

    List<Student> list(StudentQueryParam studentQueryParam);

    @Insert("insert into student(name, no, gender, phone, id_card, is_college, address, degree, graduation_date, clazz_id, create_time, update_time)\n" +
            "values(#{name}, #{no}, #{gender}, #{phone}, #{idCard}, #{isCollege}, #{address}, #{degree}, #{graduationDate}, #{clazzId}, #{createTime}, #{updateTime})")
    void insert(Student student);

    @Select("select id, name, no, gender, phone, id_card, is_college, address, degree, graduation_date, clazz_id, violation_count, violation_score, create_time, update_time" +
            " from student s where s.id=#{id}")
    Student getById(Integer id);

    void update(Student student);

    void deleteById(List<Integer> ids);

    void updateViolation(Integer id, Integer score);

    @MapKey("name")
    List<Map<String, Object>> countStudentData();

    List<Map<String, Integer>> countStudentDegreeData();
}
