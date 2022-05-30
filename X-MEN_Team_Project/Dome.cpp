#include "Dome.h"

void Dome::FadeIn(const float FRAME_TIME)
{
	if (alpha >= 1)alpha = 1;
	if (liveTime >= DomeInfo::START_TIME)return;

	alpha += FRAME_TIME / DomeInfo::START_TIME;
}

void Dome::FadeOut(const float FRAME_TIME)
{
	if (alpha <= 0)alpha = 0;
	if (liveTime <= DomeInfo::MAX_TIME - DomeInfo::END_TIME && liveTime <= 1)return;

	alpha -= FRAME_TIME / DomeInfo::END_TIME;
}

Dome::Dome()
	: liveTime(0)
	, level(1)
	, alpha(0)
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

	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 255, 255 * alpha));

	if (liveTime >= 1)
	{
		liveTime = 0;
		eraseManager = true;
	}

	liveTime += FRAME / LEVEL_MAX_TIME;
}

void Dome::Draw()
{
	AllDraw();
}

void Dome::SetLevel(const int value)
{
	if (level <= 0)level = 1;

	level = value;
}
