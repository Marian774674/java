#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// 事件总线——Observer 模式，解耦各模块间通信
// 事件类型
enum class GameEvent {
    PlayerMoved,
    CombatStarted,
    CombatEnded,
    EnemyKilled,
    ItemCollected,
    SpellSynthesized,
    LevelUp,
    FloorChanged,
    GameOver
};

class EventBus {
public:
    using Callback = std::function<void(GameEvent, const void* data)>;

    static EventBus& instance();

    // 订阅事件
    void subscribe(GameEvent event, Callback callback);

    // 发布事件
    void publish(GameEvent event, const void* data = nullptr);

    // 清除所有订阅
    void clear();

private:
    EventBus() = default;

    std::unordered_map<GameEvent, std::vector<Callback>> m_subscribers;
};
