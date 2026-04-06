package com.czx.mapper;


import com.czx.pojo.Emp;
import com.czx.pojo.EmpQueryParam;
import org.apache.ibatis.annotations.*;

import java.util.List;
import java.util.Map;

@Mapper
public interface EmpMapper {

//    ------------------------------------原始分页查询操作--------------------------------------------
//    @Select("select count(*) from emp e left join dept d on e.dept_id=d.id")
//    public long count();
//
//    @Select("select e.*,d.name deptName from emp e left join dept d on e.dept_id=d.id order by e.update_time desc limit #{start},#{pageSize}")
//    public List<Emp> list(Integer start,Integer pageSize);

//    @Select("select e.*,d.name deptName from emp e left join dept d on e.dept_id=d.id order by e.update_time desc")
    public List<Emp> list(EmpQueryParam empQueryParam);

    @Options(useGeneratedKeys=true,keyProperty="id")
    @Insert("insert into emp(username,name,gender,phone,job,salary,image,entry_date,dept_id,create_time,update_time) " +
            "values(#{username},#{name},#{gender},#{phone},#{job},#{salary},#{image},#{entryDate},#{deptId},#{createTime},#{updateTime})")
    void insert(Emp emp);

    void deleteEmp(List<Integer> ids);

    Emp getByID(Integer id);

    void updateByID(Emp emp);

    @MapKey("position")
    List<Map<String,Object>> countEmpJobData();

    @MapKey("name")
    List<Map<String, Integer>> countEmpGenderData();

    @Select("select id, username, password, name, gender,phone, job, salary, image, entry_date, dept_id, create_time, update_time from emp")
    List<Emp> findAll();

    @Select("select id, username, name from emp where username=#{username} and password=#{password}")
    Emp getUserByNameAndPassword(Emp emp);
}
