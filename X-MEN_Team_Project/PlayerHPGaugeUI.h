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
	// ���݂�HP�ɍ����悤�Ƀ��T�C�Y
	void Resize();
public:
	PlayerHPGaugeUI(MelLib::Texture* pTexture,const float* value);
	void Update() override;
	void Draw() override;

	// ���HP�Q�[�W�e�N�X�`������X�v���C�g�𐶐�
	void AddVoidTexture(MelLib::Texture* pVoidHpTexture);
};