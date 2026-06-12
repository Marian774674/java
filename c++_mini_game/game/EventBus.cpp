#include "EventBus.h"

EventBus& EventBus::instance() {
    static EventBus bus;
    return bus;
}

void EventBus::subscribe(GameEvent event, Callback callback) {
    m_subscribers[event].push_back(std::move(callback));
}

void EventBus::publish(GameEvent event, const void* data) {
    auto it = m_subscribers.find(event);
    if (it != m_subscribers.end()) {
        for (auto& cb : it->second) {
            cb(event, data);
        }
    }
}

void EventBus::clear() {
    m_subscribers.clear();
}
