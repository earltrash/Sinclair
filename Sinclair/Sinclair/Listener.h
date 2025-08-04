#pragma once
#include "pch.h"
#include "Component.h"

class ListenerComponent : public Component //약간 인터페이스 느낌으로만 만들고 싶음 
{
public:
		ListenerComponent() = default;
		virtual ~ListenerComponent() = default;

		virtual void _OnEvent(const MSG& msg) {}
		virtual bool Matches(const MSG& msg) const = 0;  // 관심 있는 메시지만 받게
};
