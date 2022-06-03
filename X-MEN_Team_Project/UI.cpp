#include "UI.h"

UI::UI()
	: sprite(MelLib::Sprite2D())
	, pTexture(nullptr)
	, scale(MelLib::Vector2(1,1))
	, angle(MelLib::Vector2(0,0))
	, pos(MelLib::Vector2(0,0))
{
}

MelLib::Vector2 UI::GetScale() const
{
	return scale;
}

MelLib::Vector2 UI::GetPosition() const
{
	return pos;
}

void UI::SetTexture(std::shared_ptr<MelLib::Texture> setTexture)
{
	pTexture = setTexture;
}

void UI::SetScale(const MelLib::Vector2& setValue)
{
	scale = setValue;
}

void UI::SetAngle(const MelLib::Vector2& setValue)
{
	angle = setValue;
}

void UI::SetPosition(const MelLib::Vector2& setValue)
{
	pos = setValue;
}
