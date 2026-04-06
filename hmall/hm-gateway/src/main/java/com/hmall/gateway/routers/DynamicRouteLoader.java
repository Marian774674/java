package com.hmall.gateway.routers;

import cn.hutool.json.JSONUtil;
import com.alibaba.cloud.nacos.NacosConfigManager;
import com.alibaba.nacos.api.config.ConfigService;
import com.alibaba.nacos.api.config.listener.Listener;
import com.alibaba.nacos.api.exception.NacosException;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.cloud.gateway.route.RouteDefinition;
import org.springframework.cloud.gateway.route.RouteDefinitionWriter;
import org.springframework.stereotype.Component;
import reactor.core.publisher.Mono;

import javax.annotation.PostConstruct;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.Executor;

@Component
@Slf4j
@RequiredArgsConstructor
public class DynamicRouteLoader {

    private final Set<String> routeIds = new HashSet<>();

    private final NacosConfigManager nacosConfigManager;

    private final RouteDefinitionWriter writer;

    private static final String dataId = "gateway-routes.json";

    private static final String group = "DEFAULT_GROUP";

    @PostConstruct
    public void initRouteConfigListener() throws NacosException {
        //1、项目启动时，先拉去一次配置，并且添加配置监听器
        ConfigService configService = nacosConfigManager.getConfigService();
        String configInfo = configService.getConfigAndSignListener(dataId, group, 5000, new Listener() {

                    @Override
                    public Executor getExecutor() {
                        return null;
                    }

                    @Override
                    public void receiveConfigInfo(String s) {
                        //2、监听到配置变更，更新路由表
                        updateRouteConfig(s);
                    }
                }
        );
        //3、第一次读取到的配置信息也需要更新到路由表
        updateRouteConfig(configInfo);
    }

    public void updateRouteConfig(String configInfo){
        log.info("监听到路由更新：{}", configInfo);
        //4、更新路由表
            //4.1、先删除旧的路由表
        for (String routeId : routeIds) {
            writer.delete(Mono.just(routeId)).subscribe();
        }
        routeIds.clear();
            //4.2、再添加新路由表
        List<RouteDefinition> list = JSONUtil.toList(configInfo, RouteDefinition.class);
        for (RouteDefinition routeDefinition : list) {
            Mono<RouteDefinition> just = Mono.just(routeDefinition);
            writer.save(just).subscribe();
            //4.3、记录路由id，方便下一次更新删除
            routeIds.add(routeDefinition.getId());
        }
    }
}
