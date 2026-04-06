package com.czx;

import java.io.FileOutputStream;
import java.io.PrintStream;
import java.lang.reflect.Field;

public class Reflect {
    public static void main(String[] args) throws Exception {
        Dog dog = new Dog("小黑", 1);
        Teacher teacher = new Teacher("小王", 18,dog);
        Student student = new Student("小张", 18,teacher);
        print( student);
        print( teacher);
        print( dog);
    }
    public static void print(Object obj) throws Exception {
        FileOutputStream fos=new FileOutputStream("javastrenghten/src/test.txt",true);
        PrintStream ps=new PrintStream(fos);
        Class c=obj.getClass();
        ps.println("============="+c.getSimpleName()+"=============");
        Field[] fields=c.getDeclaredFields();
        for (Field field : fields) {
            field.setAccessible(true);
            ps.println(field.getName()+"="+field.get(obj));
        }
        ps.close();
    }
}
