package com.czx.controller.utils;

public class CurrentHolder {

    private static final java.lang.ThreadLocal<Integer> CURRENT_LOCAL = new java.lang.ThreadLocal<>();

    public static void setCurrentId(Integer employeeId) {
        CURRENT_LOCAL.set(employeeId);
    }

    public static Integer getCurrentId() {
        return CURRENT_LOCAL.get();
    }

    public static void remove() {
        CURRENT_LOCAL.remove();
    }
}
