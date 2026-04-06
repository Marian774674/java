package com.hmdp.utils;

public interface ILock {

    /**
     * 尝试获取锁
     * @param timeoutSec 锁的过期时间，单位秒
     * @return
     */
    boolean tryLock(long timeoutSec);
    /**
     * 释放锁
     */
    void unlock();
}
