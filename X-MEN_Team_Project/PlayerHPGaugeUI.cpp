#include "PlayerHPGaugeUI.h"
#include "Player.h"

void PlayerHPGaugeUI::Resize()
{
	float gaugePercent = *gaugeValue / PlayerInitializeInfo::MAX_HP;

	scale.x = initScale.x * gaugePercent;

	// �t�H�g�V����HP�Q�[�W�̓��ꕨ�ƍő�HP��Ԃ̃e�N�X�`�������킹�Ă݂��Ƃ��̂��̍���
	const float REPOSITIONX = 17;
	pos.x = REPOSITIONX * initScale.x;
}

PlayerHPGaugeUI::PlayerHPGaugeUI(MelLib::Texture* pTexture, const float* value)
	: IGaugeUI(pTexture,value)
	, voidHpGaugeSprite(MelLib::Sprite2D())
	, voidHpGaugeTexture(nullptr)
	, initScale(scale)
	, initPos(pos)
{
}

void PlayerHPGaugeUI::Update()
{
	Resize();

	SetSpriteInfo(sprite, scale, 0, pos);
	SetSpriteInfo(voidHpGaugeSprite, initScale, 0, initPos);
}

void PlayerHPGaugeUI::Draw()
{
	voidHpGaugeSprite.Draw();
	sprite.Draw();
}

void PlayerHPGaugeUI::AddVoidTexture(MelLib::Texture* pVoidHpTexture)
{
	voidHpGaugeTexture = pVoidHpTexture;
	voidHpGaugeSprite.Create(voidHpGaugeTexture);
}
