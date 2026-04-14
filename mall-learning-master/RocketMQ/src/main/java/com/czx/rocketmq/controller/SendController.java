package com.czx.rocketmq.controller;


import com.czx.rocketmq.domain.User;
import org.apache.rocketmq.client.producer.SendCallback;
import org.apache.rocketmq.client.producer.SendResult;
import org.apache.rocketmq.common.message.Message;
import org.apache.rocketmq.spring.core.RocketMQTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.support.MessageBuilder;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.List;

@RestController
@RequestMapping("/demo")
public class SendController {
    @Autowired
    private RocketMQTemplate rocketMQTemplate;

    @GetMapping("/send")
    public String send() {
        User czx = new User("czx", 18);
        rocketMQTemplate.convertAndSend("test-topic", czx);
//        同步消息
        rocketMQTemplate.syncSend("test-topic", czx);
//        异步消息
        rocketMQTemplate.asyncSend("test-topic", czx, new SendCallback() {
            @Override
            public void onSuccess(SendResult sendResult) {
                System.out.println(sendResult);
            }

            @Override
            public void onException(Throwable throwable) {
                System.out.println(throwable);
            }
        });
//        单向消息
        rocketMQTemplate.sendOneWay("test-topic", czx);
//        延迟消息
        rocketMQTemplate.syncSend("test-topic",
                MessageBuilder.withPayload(czx).build(),
                2000,3);
//        批量消息
        List<User> msgList = new ArrayList<>();
        msgList.add(new User("czx1", 18));
        msgList.add(new User("czx2", 18));
        msgList.add(new User("czx3", 18));
        rocketMQTemplate.syncSend("test-topic", msgList,2000);

//        顺序消息
        rocketMQTemplate.asyncSendOrderly("test-topic", czx, "1", new SendCallback() {
            @Override
            public void onSuccess(SendResult sendResult) {
                System.out.println(sendResult);
            }

            @Override
            public void onException(Throwable throwable) {
                System.out.println(throwable);
            }
        });

        return "send";
    }
}
