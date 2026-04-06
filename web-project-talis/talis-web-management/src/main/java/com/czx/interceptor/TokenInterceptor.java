package com.czx.interceptor;

import com.czx.controller.utils.JwtUtils;
import com.czx.filter.TokenFilter;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Component;
import org.springframework.web.servlet.HandlerInterceptor;

import java.util.logging.Logger;


//@Component
public class TokenInterceptor implements HandlerInterceptor {

    private static final Logger log= Logger.getLogger(TokenFilter.class.getName());

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {

//        String requestURI = request.getRequestURI();
//
//        if(requestURI.contains("login")){
//            log.info("登录操作，放行");
//            return true;
//        }

        String token = request.getHeader("token");

        if(token==null||token.isEmpty()){
            log.info("令牌为空，请登录,响应401");
            response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
            return false;
        }

        try {
            JwtUtils.parseToken(token);
        } catch (Exception e) {
            log.info("令牌非法,响应401");
            response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
            return  false;
        }

        log.info("令牌合法，放行");
        return true;
    }
}
