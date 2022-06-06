#pragma once
#include "UI.h"

template <typename T>
class IGaugeUI : public UI
{
protected:
	const T* gaugeValue;
public:
	IGaugeUI(MelLib::Texture* pTexture, const T* setValue)
		: UI(pTexture)
		, gaugeValue(setValue)
	{}
	virtual ~IGaugeUI() {}
};
