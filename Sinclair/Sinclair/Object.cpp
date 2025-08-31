#pragma once
#include "pch.h"
#include "Object.h"
#include "MouseListenerComponent.h"
#include "KeyListenerComponent.h"



Object::~Object()
{
		m_Components.clear();
}
