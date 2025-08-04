#pragma once
#include "pch.h"
#include "Listener.h"
#include "InputManager.h"

using Callback = std::function<void(const MSG&)>;

inline bool InRect(const Rect& ObjP, POINT& mouseP)
{
		return (ObjP.Contains({ static_cast<float>(mouseP.x), static_cast<float>(mouseP.y) })); //결국 변환 과정에서 값이 짤릴 거는 필연적이긴 한데,,,

}

class MouseListenerComponent : public ListenerComponent //새로운 shared-raw-shared로 소유권 이중화를 막기 위함.
{
public:
		explicit MouseListenerComponent(Callback cb) : callback(std::move(cb)) {
			InputManager::Get().m_broadcaster->AddListener(this);
		}
		~MouseListenerComponent() { InputManager::Get().m_broadcaster->RemoveListener(this); } //Raii

		void _OnEvent(const MSG& msg) override {

				if (callback) callback(msg);
		};

		bool Matches(const MSG& msg)const {// 관심 있는 메시지만 받게
				return msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST;

		};
private:
		Callback callback;
};
