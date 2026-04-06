package com.czx.mapper;

import com.czx.pojo.Emp;
import com.czx.pojo.EmpExpr;
import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface EmpExprMapper {

    void insertBatch(List<EmpExpr> exprList);

    void deleteExpr(List<Integer> ids);
}
