#include "GaugeUI.h"
#include <Library.h>

void GaugeUI::CalcSize(const MelLib::Vector2& windowSize)
{
	const float DIVIDE = 100;

	float gaugePercent = value / maxValue;

	// backは常時最大サイズで描画したいからここで一旦コピー
	backDrawSize = frontDrawSize = (windowSize / DIVIDE) * sizePercent;

	// frontはゲージの値が最大値の何％のサイズになるべきだから現在の％をかける
	frontDrawSize.x *= gaugePercent;
}

GaugeUI::GaugeUI()
	: value(0)
	, maxValue(0)
	, sizePercent(MelLib::Vector2())
	, frontGauge(MelLib::Sprite2D())
	, backGauge(MelLib::Sprite2D())
	, frontDrawSize(MelLib::Vector2())
	, backDrawSize(MelLib::Vector2())
	, drawPosition(MelLib::Vector2())
	, frontColor(MelLib::Color(255,255,255,255))
	, backColor(MelLib::Color(0,0,0,255))
{
	frontGauge.Create(frontColor);
	backGauge.Create(backColor);
}

GaugeUI::~GaugeUI()
{
}

void GaugeUI::Update(float setValue)
{
	value = setValue;

	MelLib::Vector2 windowSize = MelLib::Vector2(MelLib::Library::GetWindowWidth(), MelLib::Library::GetWindowHeight());
	CalcSize(windowSize);

	frontGauge.SetScale(MelLib::Vector2(frontDrawSize));
	frontGauge.SetPosition(drawPosition);
	frontGauge.SetColor(frontColor);

	backGauge.SetScale(MelLib::Vector2(backDrawSize));
	backGauge.SetPosition(drawPosition);
	backGauge.SetColor(backColor);
}

void GaugeUI::Initialize()
{
	value = 0;
}

void GaugeUI::Draw()
{
	backGauge.Draw();
	frontGauge.Draw();
}

void GaugeUI::SetMaxValue(float setValue)
{
	maxValue = setValue;
}

void GaugeUI::SetSizePercent(const MelLib::Vector2& setSizePercent)
{
	sizePercent = setSizePercent;
}

void GaugeUI::SetPosition(const MelLib::Vector2& pos)
{
	drawPosition = pos;
}

void GaugeUI::SetFrontColor(const MelLib::Color& setColor)
{
	frontColor = setColor;
}

void GaugeUI::SetBackColor(const MelLib::Color& setColor)
{
	backColor = setColor;
}
