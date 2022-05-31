#include "Dome.h"
#include "FieldObjectWall.h"
#include "UltimateSkill.h"

void Dome::FadeIn(const float FRAME_TIME)
{
	if (liveTime >= DomeInfo::START_TIME / DomeInfo::MAX_TIME)return;

	easingValue += FRAME_TIME / DomeInfo::START_TIME;
	alpha = EaseOutQuint(easingValue);
	if (alpha >= 1)alpha = 1;
}

void Dome::FadeOut(const float FRAME_TIME)
{
	if (liveTime <= (DomeInfo::MAX_TIME - DomeInfo::END_TIME) / DomeInfo::MAX_TIME && liveTime <= 1)return;

	easingValue -= FRAME_TIME / DomeInfo::END_TIME;
	alpha = EaseOutQuint(easingValue);
	if (alpha <= 0)alpha = 0;
}

float Dome::EaseOutQuint(float t)
{
	if (t >= 1)
		return 1;

	return 1.0f - std::powf(1.0f - t, 5);
}

void Dome::CalcSize()
{
	// (フィールドの半径 - フィールドの1/10) * レベルの0 ~ 1化
	const float levelPower = ((FieldObjectWallInfo::TOP_BOTTOM_SIZE.x / 2)
		- (FieldObjectWallInfo::TOP_BOTTOM_SIZE.x / 10))
		* (level / UltimateSkillInfo::MAX_LEVEL);
	SetScale(levelPower);
}

void Dome::EndPostProcess(const float FRAME_TIME)
{
	if (IsEndTrigger())isEnd = true;
	oldIsEnd = isEnd;

	if (!isEnd)return;

	if (endElapsedTime > 1)
	{
		endElapsedTime = 0;
		isEnd = false;
	}

	endElapsedTime += FRAME_TIME / DomeEndSlowMotionInfo::TIME;
}

Dome::Dome()
	: liveTime(0)
	, level(1)
	, alpha(0)
	, easingValue(0)
	, isUse(false)
	, oldIsUse(false)
	, endElapsedTime(0)
	, isEnd(false)
	, oldIsEnd(false)
{
	// 読み込んだモデルデータを使用
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
	endElapsedTime = 0;
	isEnd = false;
	oldIsEnd = false;
}

void Dome::Update()
{
	const float FRAME_TIME = 1.0f / 60.0f;

	EndPostProcess(FRAME_TIME);

	oldIsUse = isUse;
	if (!isUse)return;

	const float LEVEL_MAX_TIME = DomeInfo::MAX_TIME * level;

	FadeIn(FRAME_TIME);
	FadeOut(FRAME_TIME);

	// アルファ値反映
	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 255, 255 * alpha));

	if (liveTime >= 1)
	{
		liveTime = 0;
		isUse = false;
		isEnd = true;
	}

	liveTime += FRAME_TIME / LEVEL_MAX_TIME;
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

bool Dome::IsPostProcessEndTrigger()
{
	return !isEnd && oldIsEnd;
}

void Dome::SetLevel(const int value)
{
	if (level <= 0)level = 1;

	level = value;

	CalcSize();
}
