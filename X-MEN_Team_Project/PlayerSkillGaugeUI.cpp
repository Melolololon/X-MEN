#include "PlayerSkillGaugeUI.h"
#include "UltimateSkill.h"

PlayerSkillGaugeUI::PlayerSkillGaugeUI(MelLib::Texture* pTexture, const int* value)
	: IGaugeUI(pTexture, value)
	, drawSprite(nullptr)
{
}

void PlayerSkillGaugeUI::Update()
{
	const float MULTIPLE_VALUE = 0.5f;
	scale = initScale * MULTIPLE_VALUE;

	size_t index = (size_t)(*gaugeValue);
	if (index >= sprites.size())return;

	drawSprite = sprites[index].get();

	SetSpriteInfo(*drawSprite, scale, 0, pos);
}

void PlayerSkillGaugeUI::Draw()
{
	if (!drawSprite)return;

	drawSprite->Draw();
}

void PlayerSkillGaugeUI::AddSprite(MelLib::Texture* pTexture)
{
	// 末尾に追加してその末尾のスプライトをテクスチャから作成
	std::shared_ptr<MelLib::Sprite2D> newSprite = std::make_shared<MelLib::Sprite2D>(pTexture);
	sprites.emplace_back(newSprite);

	ResizeWithWindow(*newSprite);
	initScale = scale;

	//newSprite.get()->SetScale(scale);
}
