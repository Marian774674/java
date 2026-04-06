package com.czx.javatbasicest;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

@Data
@AllArgsConstructor
@NoArgsConstructor

public class Student {
    private int id;
    private String name;
    private String sex;
    private LocalDateTime time;
    private String selectAddress;
}
