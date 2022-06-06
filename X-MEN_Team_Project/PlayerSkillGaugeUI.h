#pragma once
#include "IGaugeUI.h"

class PlayerSkillGaugeUI : public IGaugeUI<int>
{
private:
	MelLib::Sprite2D* drawSprite;
	std::vector<std::shared_ptr<MelLib::Sprite2D>> sprites;
public:
	PlayerSkillGaugeUI(MelLib::Texture* pTexture, const int* value);
	void Update() override;
	void Draw() override;
	void AddSprite(MelLib::Texture* pTexture);
};
