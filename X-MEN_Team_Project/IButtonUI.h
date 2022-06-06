#pragma once
#include "UI.h"

class IButtonUI : public UI
{
private:
protected:
	// UIのボタンがクリックや選択中に押された場合に動作させる処理を記述
	virtual void OnClick() = 0;
public:
	IButtonUI(MelLib::Texture* pTexture)
		: UI(pTexture) {}
	virtual ~IButtonUI() {}
};
