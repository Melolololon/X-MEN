#pragma once

namespace UltimateSkillInfo
{
	// ゲージ初期値
	const int INITIALIZE_VALUE = 0;

	// ゲージの最大値
	const int MAX_VALUE = 100;

	// レベルが上がるために必要な量
	const int LEVEL_UP_VALUE = 25;

	// 初期レベル
	const int INITIALIZE_LEVEL = 1;

	// 最大レベル
	const int MAX_LEVEL = 4;
}

class UltimateSkill
{
private:
	int value;
	int level;
	bool isUsingSkill;
private:
	// 現ゲージ量からレベルを計算
	void CalcLevel();
public:
	UltimateSkill();
	~UltimateSkill();

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
