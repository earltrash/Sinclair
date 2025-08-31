#pragma once
#include "pch.h"
#include "Listener.h"
#include "InputManager.h"
using Callback = std::function<void(const MSG&)>;


class KeyListenerComponent : public ListenerComponent
{
public:
		explicit KeyListenerComponent(Callback cb) : callback(std::move(cb)) {}
		~KeyListenerComponent() { InputManager::Get().m_broadcaster->RemoveListener(this); } //Raii


		void _OnEvent(const MSG& msg) override {
				if (callback) callback(msg);
				return;
		};
		bool Matches(const MSG& msg)const {
				return msg.message == WM_KEYDOWN || msg.message == WM_KEYUP;

		};  // 관심 있는 메시지만 받게
private:
		Callback callback;

};