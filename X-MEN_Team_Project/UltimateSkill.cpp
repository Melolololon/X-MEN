#include "UltimateSkill.h"

void UltimateSkill::CalcLevel()
{
	// ���݂̃Q�[�W�ʂ����x���A�b�v�ɕK�v�Ȋ���l�Ŋ������l
	// ��j74(���Q�[�W��) / 25(����l) = 2level
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

	// �Q�[�W�̒l���ő�l�ȏ�ɂȂ�Ȃ��悤�ɗ}��
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
