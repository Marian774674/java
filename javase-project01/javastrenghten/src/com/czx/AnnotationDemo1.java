package com.czx;

import org.junit.Test;

public class AnnotationDemo1 {
    @Test
    public void parseClass(){
        Class c1=Demo1.class;
        if(c1.isAnnotationPresent(MyBook.class)){
            MyBook myBook=(MyBook)c1.getDeclaredAnnotation(MyBook.class);
            System.out.println(myBook.value());
            System.out.println(myBook.author());
            System.out.println(myBook.price());
        }
    }
}
