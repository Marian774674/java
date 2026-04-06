package com.czx.filter;

import com.czx.controller.utils.CurrentHolder;
import com.czx.controller.utils.JwtUtils;
import io.jsonwebtoken.Claims;
import jakarta.servlet.*;
import jakarta.servlet.annotation.WebFilter;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.apache.http.HttpRequest;
import org.apache.http.impl.bootstrap.HttpServer;

import java.io.IOException;
import java.util.logging.Logger;

@WebFilter(urlPatterns = "/*")
public class TokenFilter implements Filter {

    private static final Logger log= Logger.getLogger(TokenFilter.class.getName());

    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {
        HttpServletRequest request=(HttpServletRequest)servletRequest;
        HttpServletResponse response=(HttpServletResponse)servletResponse;

        String requestURI = request.getRequestURI();

        if(requestURI.contains("login")){
            log.info("登录操作，放行");
            filterChain.doFilter(request,response);
            return;
        }

        String token = request.getHeader("token");

        if(token==null||token.isEmpty()){
            log.info("令牌为空，请登录,响应401");
            response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
            return;
        }

        try {
            Claims claims = JwtUtils.parseToken(token);
            Integer id = Integer.valueOf(claims.get("id").toString());
            CurrentHolder.setCurrentId(id);
        } catch (Exception e) {
            log.info("令牌非法,响应401");
            response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
            return;
        }

        log.info("令牌合法，放行");
        filterChain.doFilter(request,response);
        CurrentHolder.remove();
    }
}
