package com.czx;

import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.SignatureAlgorithm;
import org.junit.jupiter.api.Test;

import java.util.Date;
import java.util.HashMap;
import java.util.Map;

public class JwtTest {

    @Test
    public void testSetJWT(){
        Map<String,Object> dataMap=new HashMap<>();
        dataMap.put("id",1);
        dataMap.put("username","czx");
        String jwt = Jwts.builder()
                .signWith(SignatureAlgorithm.HS256, "Y3p4")
                .addClaims(dataMap)
                .setExpiration(new Date(System.currentTimeMillis() + 1000 * 60 * 60))
                .compact();
        System.out.println(jwt);
    }

    @Test
    public void testParseJWT(){
        String token="eyJhbGciOiJIUzI1NiJ9.eyJpZCI6MSwidXNlcm5hbWUiOiJjengiLCJleHAiOjE3NjU3OTk5NzJ9.ZeMVZDmjPgejTZpUsCPsAvXXQYkw25K9YJ24Gi8j230";
        Claims claims = Jwts.parser().setSigningKey("Y3p4")
                .parseClaimsJws(token)
                .getBody();
        System.out.println(claims);
    }
}
