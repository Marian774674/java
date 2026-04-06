package com.czx.mapper;

import com.czx.pojo.User;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper// 这个注解表示这个接口被mybatis管理，mybatis会根据这个接口生成代理类
public interface UserMapper {
    public List<User> findAll();
}
