#pragma once
#include "FieldObject.h"

namespace FieldObjectWallInfo
{
	// 0 ~ 1
	const float SCALE = 0.075f;
	const MelLib::Vector3 TOP_BOTTOM_SIZE = MelLib::Vector3(1500, 128, 100) * SCALE;
	const MelLib::Vector3 RIGHT_LEFT_SIZE = MelLib::Vector3(100, 128, 1500) * SCALE;
}

class FieldObjectWall : public FieldObject
{
private:
public:
	FieldObjectWall();

	void Update() override;
	void Draw() override;
};
