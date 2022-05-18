#pragma once
#include <Sprite2D.h>

// プレイヤーのHPゲージなどのゲージUIをスプライト描画するクラス
class GaugeUI
{
private:
	// ゲージの量
	float value;

	// ゲージの最大値
	float maxValue;

	// ウィンドウに対する大きさ
	MelLib::Vector2 sizePercent;

	// ゲージの量を描画をする用
	MelLib::Sprite2D frontGauge;

	// ゲージの最大値を描画する用
	MelLib::Sprite2D backGauge;

	MelLib::Vector2 frontDrawSize;
	MelLib::Vector2 backDrawSize;

	MelLib::Vector2 drawPosition;

	MelLib::Color frontColor;
	MelLib::Color backColor;
private:
	// windowのサイズから描画サイズを計算
	void CalcSize(const MelLib::Vector2& windowSize);
public:
	GaugeUI();
	~GaugeUI();
	void Update(float setValue);
	void Initialize();
	void Draw();

#pragma region Setter
	// ゲージの最大値をセット
	void SetMaxValue(float setValue);

	// ゲージのサイズをウィンドウに対してどれくらいにするか
	// 引数 : 0 ~ 100
	void SetSizePercent(const MelLib::Vector2& setSizePercent);

	void SetPosition(const MelLib::Vector2& pos);
	void SetFrontColor(const MelLib::Color& setColor);
	void SetBackColor(const MelLib::Color& setColor);
#pragma endregion
};
