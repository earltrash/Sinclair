#include "Button.h"

Button::Button(std::string name, Rect rect)
{
	Vec2 pos = { rect.x, rect.y };
	SetPosition(pos);
}

Button::~Button()
{
}
