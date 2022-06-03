#include "UltimateSkill.h"
#include "Library.h"
#include <Input.h>
#include <GameObjectManager.h>

void UltimateSkill::CalcLevel()
{
	// ���݂̃Q�[�W�ʂ����x���A�b�v�ɕK�v�Ȋ���l�Ŋ������l
	// ��j74(���Q�[�W��) / 25(����l) = 2level
	level = (int)(value / UltimateSkillInfo::LEVEL_UP_VALUE);
}

void UltimateSkill::CalcSize(const MelLib::Vector2& windowSize)
{
	const float DIVIDE = 100;

	float gaugePercent = value / UltimateSkillInfo::MAX_VALUE;

	// back�͏펞�ő�T�C�Y�ŕ`�悵�������炱���ň�U�R�s�[
	backDrawSize = frontDrawSize = (windowSize / DIVIDE) * UltimateSkillGaugeDrawInfo::SIZE_PERCENT;

	// front�̓Q�[�W�̒l���ő�l�̉����̃T�C�Y�ɂȂ�ׂ������猻�݂́���������
	frontDrawSize.x *= gaugePercent;
}

void UltimateSkill::CalcPosition(const MelLib::Vector2& windowSize)
{
	float subWidth = windowSize.x - backDrawSize.x;
	float drawPosX = subWidth / 2;

	drawPosition.x = drawPosX;
}

void UltimateSkill::CalcColor()
{
	drawColor.r = (unsigned char)(UltimateSkillGaugeDrawInfo::FRONT_COLOR.r * (level / UltimateSkillInfo::MAX_LEVEL));
	drawColor.g = (unsigned char)(UltimateSkillGaugeDrawInfo::FRONT_COLOR.g * (level / UltimateSkillInfo::MAX_LEVEL));
	drawColor.b = (unsigned char)(UltimateSkillGaugeDrawInfo::FRONT_COLOR.b * (level / UltimateSkillInfo::MAX_LEVEL));
}

UltimateSkill::UltimateSkill()
	: value((int)UltimateSkillInfo::INITIALIZE_VALUE)
	, level((int)UltimateSkillInfo::INITIALIZE_LEVEL)
	, isUsingSkill(false)
	, frontGauge(MelLib::Sprite2D())
	, backGauge(MelLib::Sprite2D())
	, frontDrawSize(MelLib::Vector2())
	, backDrawSize(MelLib::Vector2())
	, drawPosition(MelLib::Vector2())
	, drawColor(UltimateSkillGaugeDrawInfo::FRONT_COLOR)
{
	frontGauge.Create(UltimateSkillGaugeDrawInfo::FRONT_COLOR);
	backGauge.Create(UltimateSkillGaugeDrawInfo::BACK_COLOR);
}

UltimateSkill::~UltimateSkill()
{
}

void UltimateSkill::Update()
{
	if (dome)
	{
		if (!dome.get()->IsUse())
		{
			isUsingSkill = false;
		}
	}

	MelLib::Vector2 windowSize = MelLib::Vector2((float)MelLib::Library::GetWindowWidth(), (float)MelLib::Library::GetWindowHeight());
	if (MelLib::Input::KeyTrigger(DIK_UP))
	{
		AddValue(UltimateSkillInfo::LEVEL_UP_VALUE);
	}

	CalcSize(windowSize);
	//CalcPosition(windowSize);
	drawPosition = UltimateSkillGaugeDrawInfo::DRAW_POSITION;
	CalcColor();

	frontGauge.SetScale(MelLib::Vector2(frontDrawSize));
	frontGauge.SetPosition(drawPosition);
	frontGauge.SetColor(drawColor);

	backGauge.SetScale(MelLib::Vector2(backDrawSize));
	backGauge.SetPosition(drawPosition);
	backGauge.SetColor(UltimateSkillGaugeDrawInfo::BACK_COLOR);
}

void UltimateSkill::Draw()
{
	backGauge.Draw();
	frontGauge.Draw();
}

void UltimateSkill::Use(const MelLib::Vector3& pos)
{
	CalcLevel();
	if (level <= 0)return;

	if (!dome)return;

	dome.get()->SetPosition(pos);
	dome.get()->Initialize();
	dome.get()->SetLevel(level);

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

void UltimateSkill::SetDome(std::shared_ptr<Dome> setDome)
{
	dome = setDome;
}
