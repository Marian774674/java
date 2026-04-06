package com.czx.mapper;

import com.czx.pojo.Clazz;
import com.czx.pojo.ClazzQueryParam;
import org.apache.ibatis.annotations.Delete;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;

import java.util.List;

@Mapper
public interface ClazzMapper {

    public List<Clazz> list(ClazzQueryParam clazzQueryParam);

    @Delete("delete from clazz where clazz.id=#{id}")
    void deleteClazz(Integer id);

    void insertClazz(Clazz clazz);


    @Select("select id, name, room, begin_date, end_date, master_id, subject, create_time, update_time from clazz where clazz.id=#{id}")
    Clazz getById(Integer id);

    void updateClazz(Clazz clazz);

    @Select("select id, name, room, begin_date, end_date, master_id, subject, create_time, update_time from clazz")
    List<Clazz> findAll();

    @Select("select count(*) from student where clazz_id=#{id}")
    int countStudent(Integer id);
}
