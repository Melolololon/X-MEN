#include "UltimateSkill.h"

void UltimateSkill::CalcLevel()
{
	// 現在のゲージ量をレベルアップに必要な既定値で割った値
	// 例）74(現ゲージ量) / 25(既定値) = 2level
	level = value / UltimateSkillInfo::LEVEL_UP_VALUE;
}

UltimateSkill::UltimateSkill()
	: value(UltimateSkillInfo::INITIALIZE_VALUE)
	, level(UltimateSkillInfo::INITIALIZE_LEVEL)
	, isUsingSkill(false)
{
}

UltimateSkill::~UltimateSkill()
{
}

void UltimateSkill::Use()
{
	// value -= ****

	CalcLevel();

	isUsingSkill = true;
}

void UltimateSkill::AddValue(int addValue)
{
	value += addValue;

	// ゲージの値を最大値以上にならないように抑制
	if (value >= UltimateSkillInfo::MAX_VALUE)value = UltimateSkillInfo::MAX_VALUE;

	CalcLevel();
}

bool UltimateSkill::GetIsUsingSkill()const
{
	return isUsingSkill;
}

void UltimateSkill::SetIsUsingSkill(bool flag)
{
	isUsingSkill = flag;
}
