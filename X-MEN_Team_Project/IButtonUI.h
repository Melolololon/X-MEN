#pragma once
#include "UI.h"

class IButtonUI : public UI
{
private:
protected:
	// UI�̃{�^�����N���b�N��I�𒆂ɉ����ꂽ�ꍇ�ɓ��삳���鏈�����L�q
	virtual void OnClick() = 0;
public:
	IButtonUI(MelLib::Texture* pTexture)
		: UI(pTexture) {}
	virtual ~IButtonUI() {}
};
