package com.czx;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.servlet.ServletComponentScan;

@ServletComponentScan
@SpringBootApplication
public class TalisWebManagementApplication {

    public static void main(String[] args) {
        SpringApplication.run(TalisWebManagementApplication.class, args);
    }

}
