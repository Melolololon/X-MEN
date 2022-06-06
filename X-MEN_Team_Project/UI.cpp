#include "UI.h"
#include <Library.h>

void UI::ResizeWithWindow(const MelLib::Sprite2D& resizeSprite)
{
	// ���݂�Window�T�C�Y�ƍő�l����Window�̃T�C�Y���ς���Ă��e�N�X�`���̃T�C�Y�ŕ`��ł���悤��
	const MelLib::Vector2 MAX_RESOLUTION = { 1920.0f,1080.0f };
	const MelLib::Vector2 CURRENT_RESOLUTION = { (float)MelLib::Library::GetWindowWidth()
												,(float)MelLib::Library::GetWindowHeight() };
	const MelLib::Vector2 WINDOW_SCALE_RATE = CURRENT_RESOLUTION / MAX_RESOLUTION;

	scale = WINDOW_SCALE_RATE;
}

void UI::SetSpriteInfo(MelLib::Sprite2D& reinfoSprite, const MelLib::Vector2& setScale, float setAngle, const MelLib::Vector2& setPos)
{
	reinfoSprite.SetScale(setScale);
	sprite.SetAngle(setAngle);
	reinfoSprite.SetPosition(setPos);
}

UI::UI(MelLib::Texture* pTexture)
	: sprite(MelLib::Sprite2D())
	, pTexture(pTexture)
	, scale(sprite.GetScale())
	, angle(sprite.GetAngle())
	, pos(sprite.GetPosition())
{
	if (!this->pTexture)return;
	// �e�N�X�`������X�v���C�g���쐬(�T�C�Y�̓e�N�X�`���̃T�C�Y�ɂȂ�̂ł��̌��Window�̔䗦�ɕς���)
	sprite.Create(pTexture);

	ResizeWithWindow(sprite);
}

void UI::Initialize()
{
}

void UI::Update()
{
	SetSpriteInfo(sprite,scale,0,pos);
}

void UI::Draw()
{
	sprite.Draw();
}

MelLib::Vector2 UI::GetScale() const
{
	return scale;
}

MelLib::Vector2 UI::GetPosition() const
{
	return pos;
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
