#pragma once
#include "pch.h"

class Object;

class Component
{
public:
		virtual ~Component() = default;
		virtual void Update() {}
		virtual void FixedUpdate(float dt) {}
		virtual void OnEvent(const std::string& ev) {}

		void SetOwner(Object* owner) { m_Owner = owner; }
		Object* m_Owner = nullptr;
};