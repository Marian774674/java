package com.czx.pojo;

import java.util.List;

public class ClazzNumber {
    private List clazzList;
    private List dataList;

    public ClazzNumber(List clazzList, List dataList) {
        this.clazzList = clazzList;
        this.dataList = dataList;
    }

    public List getClazzList() {
        return clazzList;
    }

    public void setClazzList(List clazzList) {
        this.clazzList = clazzList;
    }

    public List getDataList() {
        return dataList;
    }

    public void setDataList(List dataList) {
        this.dataList = dataList;
    }
}
