#pragma once
#include "pch.h"
#include "Listener.h"
#include "InputManager.h"

using Callback = std::function<void(const MSG&)>;

inline bool InRect(const Rect& ObjP, POINT& mouseP)
{
		return (ObjP.Contains({ static_cast<float>(mouseP.x), static_cast<float>(mouseP.y) })); //�ᱹ ��ȯ �������� ���� ©�� �Ŵ� �ʿ����̱� �ѵ�,,,

}

class MouseListenerComponent : public ListenerComponent //���ο� shared-raw-shared�� ������ ����ȭ�� ���� ����.
{
public:
		explicit MouseListenerComponent(Callback cb) : callback(std::move(cb)) {
			InputManager::Get().m_broadcaster->AddListener(this);
		}
		~MouseListenerComponent() { InputManager::Get().m_broadcaster->RemoveListener(this); } //Raii

		void _OnEvent(const MSG& msg) override {

				if (callback) callback(msg);
		};

		bool Matches(const MSG& msg)const {// ���� �ִ� �޽����� �ް�
				return msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST;

		};
private:
		Callback callback;
};
