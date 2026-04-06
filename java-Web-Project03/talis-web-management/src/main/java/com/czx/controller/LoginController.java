package com.czx.controller;


import com.czx.pojo.Emp;
import com.czx.pojo.LoginInfo;
import com.czx.pojo.Result;
import com.czx.service.EmpService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class LoginController {

    private static final Logger log = LoggerFactory.getLogger(LoginController.class);

    @Autowired
    private EmpService empService;

    @PostMapping("/login")
    public Result login(@RequestBody Emp emp) {
        log.info("员工登录：{}",emp);
        LoginInfo loginInfo =empService.login(emp);
        if(loginInfo!=null) {
            return Result.success(loginInfo);
        }
        return Result.error("用户名或密码错误！");
    }
}
