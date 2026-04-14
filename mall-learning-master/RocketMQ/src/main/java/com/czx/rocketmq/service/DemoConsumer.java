package com.czx.rocketmq.service;

import com.czx.rocketmq.domain.User;
import org.apache.rocketmq.spring.annotation.MessageModel;
import org.apache.rocketmq.spring.annotation.RocketMQMessageListener;
import org.apache.rocketmq.spring.annotation.SelectorType;
import org.apache.rocketmq.spring.core.RocketMQListener;
import org.springframework.stereotype.Service;

@Service
@RocketMQMessageListener(topic = "test-topic",
        consumerGroup = "demo-consumer-group",
        selectorType = SelectorType.SQL92, selectorExpression = "age > 18",
        messageModel = MessageModel.BROADCASTING)
public class DemoConsumer implements RocketMQListener<User> {

    @Override
    public void onMessage(User user) {
        System.out.println("receive message:" + user);
    }
}
