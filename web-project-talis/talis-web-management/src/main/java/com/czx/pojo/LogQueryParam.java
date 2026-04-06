package com.czx.pojo;

public class LogQueryParam {
    private Integer page;
    private Integer pageSize;

    public LogQueryParam() {
    }

    public LogQueryParam(Integer page, Integer pageSize) {
        this.page = page;
        this.pageSize = pageSize;
    }

    public Integer getPage() {
        return page;
    }

    public void setPage(Integer page) {
        this.page = page;
    }

    public Integer getPageSize() {
        return pageSize;
    }

    public void setPageSize(Integer pageSize) {
        this.pageSize = pageSize;
    }
}
