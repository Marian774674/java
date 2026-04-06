package com.itheima.publisher.config;

import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.amqp.core.ReturnedMessage;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.context.annotation.Configuration;

import javax.annotation.PostConstruct;

@Slf4j
@Configuration
@RequiredArgsConstructor
public class MqConfig {

    private final RabbitTemplate rabbitTemplate;

    @PostConstruct
    public void init(){
         rabbitTemplate.setReturnsCallback(returnedMessage -> {
             log.error("监听到消息Return callback");
             log.info("交换机:{}", returnedMessage.getExchange());
             log.info("routingKey:{}", returnedMessage.getRoutingKey());
             log.info("message:{}", returnedMessage.getMessage());
             log.info("replyCode:{}", returnedMessage.getReplyCode());
             log.info("replyText:{}", returnedMessage.getReplyText());
         });
    }
}
