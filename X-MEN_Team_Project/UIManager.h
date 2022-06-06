#pragma once
#include "UI.h"
#include "IButtonUI.h"
#include "IGaugeUI.h"
#include <vector>

class UIManager
{
private:
	// ゲージなどのただ描画されているだけのUIの基底クラス配列
	std::vector<std::shared_ptr<UI>> uies;
	// ポーズボタンなどのクリックや特定のボタンを押した際に動作する系のUIの基底クラス配列
	std::vector<std::shared_ptr<UI>> buttonUies;
public:
	UIManager();
	~UIManager();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	// テクスチャとUIのタイプから新しいUIを追加
	void AddUI(std::shared_ptr<UI> newUI);

	template <typename T>
	void AddGaugeUI(std::shared_ptr<IGaugeUI<T>> newUI);

	void AddButtonUI(std::shared_ptr<IButtonUI> newUI);
};

template<typename T>
inline void UIManager::AddGaugeUI(std::shared_ptr<IGaugeUI<T>> newUI)
{
	uies.push_back(newUI);
}
