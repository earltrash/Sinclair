#pragma once
#include "Object.h"
#include "pch.h"

class Button : public Object
{
public:
		Button(std::string name, Rect rect);
		~Button();

		std::string name;
};

