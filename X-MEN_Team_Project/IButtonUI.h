#pragma once
#include "UI.h"

class IButtonUI : public UI
{
private:
public:
	// UIのボタンがクリックや選択中に押された場合に動作させる処理を記述
	virtual void OnClick() {}
};
