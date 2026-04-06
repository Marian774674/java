package com.itheima.publisher;

import lombok.extern.slf4j.Slf4j;
import org.junit.jupiter.api.Test;
import org.springframework.amqp.rabbit.connection.CorrelationData;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.util.concurrent.ListenableFutureCallback;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

@Slf4j
@SpringBootTest
class SpringAMQPTest {

    @Autowired
    private RabbitTemplate rabbitTemplate;
    @Test
    public void testSimpleQueue(){
        String message = "hello spring amqp";

        String queueName = "simple.queue";

        rabbitTemplate.convertAndSend(queueName, message);
    }

    @Test
    public void testWorkQueue(){
        for (int i = 0; i < 50; i++) {
            String message = "hello spring amqp"+ i;

            String queueName = "work.queue";

            rabbitTemplate.convertAndSend(queueName, message);
        }
    }

    @Test
    public void testFanoutQueue(){
        String message = "hello spring amqp";

        String exchangeName = "hmall.fanout";

        rabbitTemplate.convertAndSend(exchangeName, "", message);
    }
    @Test
    public void testDirectQueue(){
        String message = "hello spring amqp";

        String exchangeName = "hmall.direct";

        rabbitTemplate.convertAndSend(exchangeName, "blue", message);
    }

    @Test
    public void testTopicQueue(){
        String message = "hello spring amqp";

        String exchangeName = "hmall.topic";

        rabbitTemplate.convertAndSend(exchangeName, "china.weather", message);
    }

    @Test
    public void testSendObject(){
        Map<String,Object> msg = new HashMap<>();

        msg.put("name", "jack");
        msg.put("age", 18);
        rabbitTemplate.convertAndSend("Object.queue", msg);
    }

    @Test
    public void testConfirmCallback() throws InterruptedException {
        //1.创建correlationData
        CorrelationData cd = new CorrelationData(UUID.randomUUID().toString());
        //2.设置回调函数
        cd.getFuture().addCallback(new ListenableFutureCallback<CorrelationData.Confirm>() {
            @Override
            public void onFailure(Throwable ex) {
                log.error("消息发送失败", ex);
            }

            @Override
            public void onSuccess(CorrelationData.Confirm result) {
                //判断消息是否发送成功
                if (result.isAck()) {
                    log.info("收到confirmCallback ack，消息发送成功");
                }else{
                    log.error("收到confirmCallback nack，消息发送失败:{}",result.getReason());
                }
            }
        });
        String message = "hello spring amqp";

        String exchangeName = "hmall.direct";

        rabbitTemplate.convertAndSend(exchangeName, "blue1", message,cd);

        Thread.sleep(5000);
    }
}