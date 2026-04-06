package com.hmall.api.config;

import com.hmall.api.client.ItemClient;
import com.hmall.api.client.fallback.ItemClientFallBackFactory;
import com.hmall.common.utils.UserContext;
import feign.Logger;
import feign.RequestInterceptor;
import feign.RequestTemplate;
import org.springframework.context.annotation.Bean;

public class DefaultFeignConfig {
    @Bean
    public Logger.Level feignLoggerLevel(){
        return Logger.Level.FULL;
    }

    @Bean
    public RequestInterceptor userInfoRequestInterceptor(){
        return new RequestInterceptor() {
            @Override
            public void apply(RequestTemplate requestTemplate) {
                requestTemplate.header("user-info", UserContext.getUser().toString());
            }
        };
    }

    @Bean
    public ItemClientFallBackFactory itemClientFallBackFactory(){
        return new ItemClientFallBackFactory();
    }
}
