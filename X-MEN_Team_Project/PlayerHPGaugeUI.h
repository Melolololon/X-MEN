#pragma once
#include "IGaugeUI.h"

class PlayerHPGaugeUI : public IGaugeUI<float>
{
private:
	MelLib::Sprite2D voidHpGaugeSprite;
	MelLib::Texture* voidHpGaugeTexture;
	MelLib::Vector2 initScale;
	MelLib::Vector2 initPos;
protected:
	// 現在のHPに合うようにリサイズ
	void Resize();
public:
	PlayerHPGaugeUI(MelLib::Texture* pTexture,const float* value);
	void Update() override;
	void Draw() override;

	// 空のHPゲージテクスチャからスプライトを生成
	void AddVoidTexture(MelLib::Texture* pVoidHpTexture);
};