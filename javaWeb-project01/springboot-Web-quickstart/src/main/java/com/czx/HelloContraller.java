package com.czx;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloContraller {
    @RequestMapping("/hello")
    public String hello(String name){
        System.out.println("name:"+ name);
        return "hello"+name+"~";
    }
}
