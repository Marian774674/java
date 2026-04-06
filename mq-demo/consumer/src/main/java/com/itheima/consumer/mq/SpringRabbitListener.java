package com.itheima.consumer.mq;

import lombok.extern.slf4j.Slf4j;
import org.springframework.amqp.rabbit.annotation.Exchange;
import org.springframework.amqp.rabbit.annotation.Queue;
import org.springframework.amqp.rabbit.annotation.QueueBinding;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;

import java.util.Map;

@Slf4j
@Component
public class SpringRabbitListener {

    @RabbitListener(queues = "work.queue")
    public void ListenWorkQueue1(String msg) throws InterruptedException {
        log.info("消费者1 接收到消息：{}", msg);
        Thread.sleep(25);
    }

    @RabbitListener(queues = "work.queue")
    public void ListenWorkQueue2(String msg) throws InterruptedException {
        log.error("消费者2 接收到消息：{}", msg);
        Thread.sleep(200);
    }

    @RabbitListener(queues = "fanout.queue1")
    public void ListenFanoutQueue1(String msg) {
        log.info("消费者1 接收到消息：{}", msg);
    }

    @RabbitListener(queues = "fanout.queue2")
    public void ListenFanoutQueue2(String msg) {
        log.error("消费者2 接收到消息：{}", msg);
    }
    @RabbitListener(bindings = @QueueBinding(
            value = @Queue(name = "direct.queue1"),
            exchange = @Exchange(name = "hmall.direct"),
            key = {"blue","red"}
    ))
    public void ListenDirectQueue1(String msg) {
        log.info("消费者1 接收到消息：{}", msg);
    }

    @RabbitListener(bindings = @QueueBinding(
            value = @Queue(name = "direct.queue2"),
            exchange = @Exchange(name = "hmall.direct"),
            key = {"yellow","red"}
    ))
    public void ListenDirectQueue2(String msg) {
        log.error("消费者2 接收到消息：{}", msg);
    }

    @RabbitListener(queues = "topic.queue1")
    public void ListenTopicQueue1(String msg) {
        log.info("消费者1 接收到消息：{}", msg);
    }

    @RabbitListener(queues = "topic.queue2")
    public void ListenTopicQueue2(String msg) {
        log.error("消费者2 接收到消息：{}", msg);
    }

    @RabbitListener(queues = "Object.queue")
    public void ListenObjectQueue(Map<String,Object> msg) {
        log.info("消费者 接收到消息：{}", msg);
    }
}
