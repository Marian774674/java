package com.hmall.common.interceptors;

import cn.hutool.core.util.StrUtil;
import com.hmall.common.utils.UserContext;
import org.springframework.web.servlet.HandlerInterceptor;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class UserInfoInterceptor implements HandlerInterceptor {

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception{
        //1、获取登录用户信息
        String userInfo = request.getHeader("user-info");
        //2、判断用户信息是否为空，为空则不处理
        if(StrUtil.isNotBlank(userInfo)){
            //3、不为空则设置用户信息
            UserContext.setUser(Long.valueOf(userInfo));
        }
        //4、放行
        return true;
    }

    @Override
    public void afterCompletion(HttpServletRequest request,HttpServletResponse response,Object handler,Exception ex) throws Exception {
        // 5、清理用户信息
        UserContext.removeUser();
    }
}
