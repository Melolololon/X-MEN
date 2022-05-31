#include "Dome.h"
#include "FieldObjectWall.h"
#include "UltimateSkill.h"

void Dome::FadeIn(const float FRAME_TIME)
{
	if (liveTime >= DomeInfo::START_TIME / DomeInfo::MAX_TIME)return;

	easingValue += FRAME_TIME / DomeInfo::START_TIME;
	alpha = EaseOutQuint(0, 1, easingValue);
	if (alpha >= 1)alpha = 1;
}

void Dome::FadeOut(const float FRAME_TIME)
{
	if (liveTime <= (DomeInfo::MAX_TIME - DomeInfo::END_TIME) / DomeInfo::MAX_TIME && liveTime <= 1)return;

	easingValue -= FRAME_TIME / DomeInfo::END_TIME;
	alpha = EaseOutQuint(1, 0, easingValue);
	if (alpha <= 0)alpha = 0;
}

float Dome::EaseOutQuint(float s, float e, float t)
{
	if (t >= 1)
		return 1;

	return 1.0f - std::powf(1.0f - t, 5);
}

void Dome::CalcSize()
{
	// (�t�B�[���h�̔��a - �t�B�[���h��1/10) * ���x����0 ~ 1��
	const float levelPower = ((FieldObjectWallInfo::TOP_BOTTOM_SIZE.x / 2) 
							- (FieldObjectWallInfo::TOP_BOTTOM_SIZE.x / 10))
							* (level / UltimateSkillInfo::MAX_LEVEL);
	SetScale(levelPower);
}

Dome::Dome()
	: liveTime(0)
	, level(1)
	, alpha(0)
	, easingValue(0)
	, isUse(false)
	, oldIsUse(false)
{
	// �ǂݍ��񂾃��f���f�[�^���g�p
	const float SCALE = 2;
	const float MODEL_SIZE = 2 * SCALE;
	modelObjects["main"].Create(MelLib::ModelData::Get("domeObj"));
	modelObjects["main"].SetScale(MODEL_SIZE);

	SetScale(MODEL_SIZE);
}

Dome::~Dome()
{
}

void Dome::Initialize()
{
	liveTime = 0;
	level = 1;
	alpha = 0;
	easingValue = 0;
	isUse = true;
	oldIsUse = false;
}

void Dome::Update()
{
	oldIsUse = isUse;

	if (!isUse)return;

	const float FRAME = 1.0f / 60.0f;
	const float LEVEL_MAX_TIME = DomeInfo::MAX_TIME * level;

	FadeIn(FRAME);
	FadeOut(FRAME);

	// �A���t�@�l���f
	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 255, 255 * alpha));

	if (liveTime >= 1)
	{
		liveTime = 0;
		isUse = false;
	}

	liveTime += FRAME / LEVEL_MAX_TIME;
}

void Dome::Draw()
{
	if (!isUse)return;
	AllDraw();
}

bool Dome::IsUse()
{
	return isUse;
}

bool Dome::IsEndTrigger()
{
	return !isUse && oldIsUse;
}

void Dome::SetLevel(const int value)
{
	if (level <= 0)level = 1;

	level = value;

	CalcSize();
}
