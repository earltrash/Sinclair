#pragma once
#include "pch.h"
#include "Listener.h"

using RawEventCallback = std::function<void(const MSG&)>;

class EventDispatcher {
public:

    EventDispatcher() = default;
    ~EventDispatcher() {

        // listeners.clear();
    };

    void AddListener(ListenerComponent* listener) {
        listeners.push_back(listener);
    }

    void RemoveListener(ListenerComponent* listener) {
        listeners.erase(std::remove(listeners.begin(), listeners.end(), listener),
            listeners.end());
    }

    void Broadcast(const MSG& msg) {
        for (auto* listener : listeners) {
            if (listener && listener->Matches(msg)) {
                listener->_OnEvent(msg);
            }
        }
    }

    void clean() {
        listeners.clear();
    }

private:
    std::vector<ListenerComponent*> listeners;
    // m_pendingRemove 제거!
};

//    void AddListener(ListenerComponent* listener) { //IListener를 그냥 
//        listeners.push_back(listener);
//    }
//
//    void RemoveListener(ListenerComponent* listener) {
//
//        listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
//            [listener](ListenerComponent* ptr) {
//                return ptr == listener;
//            }),
//            listeners.end());
//
//        m_pendingRemove.emplace(listener);
//    }// 객체로 관리하니, 이런식으로 관리도 편하게 할 수 있음, 한 번에 하나만 하자.
//
//    void Broadcast(const MSG& msg) {
//
//        for (size_t i = 0; i < listeners.size(); ++i) {
//            ListenerComponent* l = listeners[i];
//
//            if (!l) continue;
//
//            if (std::find(m_pendingRemove.begin(), m_pendingRemove.end(), l) != m_pendingRemove.end())
//                continue;
//
//            // 3. 실제 살아있는 리스너만 메시지 처리
//            if (l->Matches(msg))
//                l->_OnEvent(msg);
//        }
//
//        for (auto& ptr : m_pendingRemove) {
//            listeners.erase(std::remove(listeners.begin(), listeners.end(), ptr), listeners.end());
//        }
//        m_pendingRemove.clear();
//    }
//
//    void ListnerCount()
//    {
//        std::cout << listeners.size() << std::endl;
//    }
//
//    void clean()
//        //갑자기 끄거나, End Scene에서 게임 강제로 끈 경우에 생길 수 있기 때문에 만듦 
//        //원래 이 자원관리는 Scene 단위라 windowquit에 호출될 일은 x 그래도 그냥 보험으로...
//    {
//        for (auto& e : listeners)
//        {
//            RemoveListener(e);
//        }
//
//        for (auto& ptr : m_pendingRemove) {
//            listeners.erase(std::remove(listeners.begin(), listeners.end(), ptr), listeners.end());
//        }
//        m_pendingRemove.clear();
//    }
//
//public:
//
//    std::vector<ListenerComponent*> listeners;
//    std::unordered_set<ListenerComponent*> m_pendingRemove; //use - after -free 방지
//};
