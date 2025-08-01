#pragma once
#include "pch.h"
#include "Object.h"
#include "Status.h"
#include "SimpleMathHelper.h"
using namespace D2DTM;

class UIWindow	: public Object
{
public:
		UIWindow() = default;
		virtual ~UIWindow();

		void Update() override;;
		void Render() override {};
		virtual bool HandleMouseHover(Vec2 mousePos) = 0;
		virtual UIWindowType GetType() = 0;

		virtual void SetActivate(bool active) { m_isActive = active; }


private:

		UIWindowType m_windowType;
		Vec2 m_position;
		Vec2 m_size;
		bool m_isActive = false;
		bool isDragging = false;
};

