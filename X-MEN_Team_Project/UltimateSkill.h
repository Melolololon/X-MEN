#pragma once
#include <Sprite2D.h>

namespace UltimateSkillInfo
{
	// ゲージ初期値
	const float INITIALIZE_VALUE = 0.0f;

	// ゲージの最大値
	const float MAX_VALUE = 100.0f;

	// レベルが上がるために必要な量
	const float LEVEL_UP_VALUE = 25.0f;

	// 初期レベル
	const float INITIALIZE_LEVEL = 1.0f;

	// 最大レベル
	const float MAX_LEVEL = 4.0f;
}

namespace UltimateSkillGaugeDrawInfo
{
	// windowのサイズに対する比率 0 ~ 100
	const MelLib::Vector2 SIZE_PERCENT = MelLib::Vector2(10, 2);

	// 描画色
	const MelLib::Color FRONT_COLOR = MelLib::Color(255, 0, 0, 255);
	const MelLib::Color BACK_COLOR = MelLib::Color(64, 64, 64, 255);

	const MelLib::Vector2 DRAW_POSITION = MelLib::Vector2(20, 125);
}

class UltimateSkill
{
private:
	int value;
	int level;
	bool isUsingSkill;

	// ゲージの量を描画をする用
	MelLib::Sprite2D frontGauge;

	// ゲージの最大値を描画する用
	MelLib::Sprite2D backGauge;

	MelLib::Vector2 frontDrawSize;
	MelLib::Vector2 backDrawSize;

	MelLib::Vector2 drawPosition;
	MelLib::Color drawColor;
private:
	// 現ゲージ量からレベルを計算
	void CalcLevel();

	// windowのサイズから描画するサイズを計算する
	void CalcSize(const MelLib::Vector2& windowSize);

	// windowのサイズから描画する位置を計算する
	void CalcPosition(const MelLib::Vector2& windowSize);

	void CalcColor();
public:
	UltimateSkill();
	~UltimateSkill();

	void Update();
	void Draw();

	// 今のところ値を使用したあとにレベルを再計算
	void Use();

	// ゲージの値を増やす
	void AddValue(int addValue);

#pragma region Getter
	// スキルを使用しているか取得
	bool GetIsUsingSkill() const;
#pragma endregion

#pragma region Setter
	// スキルを使用しているフラグを書き換える
	void SetIsUsingSkill(bool flag);
#pragma endregion
};
