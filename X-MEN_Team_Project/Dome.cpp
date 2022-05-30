#include "Dome.h"

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

Dome::Dome()
	: liveTime(0)
	, level(1)
	, alpha(0)
	, easingValue(0)
	, isUse(true)
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

void Dome::Update()
{
	const float FRAME = 1.0f / 60.0f;
	const float LEVEL_MAX_TIME = DomeInfo::MAX_TIME * level;

	FadeIn(FRAME);
	FadeOut(FRAME);

	// アルファ値反映
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
	AllDraw();
}

void Dome::EraseFunc()
{
	eraseManager = true;
}

bool Dome::IsUse()
{
	return isUse;
}

void Dome::SetLevel(const int value)
{
	if (level <= 0)level = 1;

	level = value;
}
