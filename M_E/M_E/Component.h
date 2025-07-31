#pragma once
#include "pch.h"

class Object;

class Component
{
public:
		virtual ~Component() = default;
		void SetOwner(Object* owner) { m_Owner = owner; }
		Object* m_Owner = nullptr;
};