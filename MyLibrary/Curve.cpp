#include "Curve.h"

using namespace MelLib;
void Curve::SetPoints(const std::vector<Vector2>& points)
{
	auto pointsSize = points.size();
	this->points.resize(pointsSize);
	for (int i = 0; i < pointsSize; i++) 
	{
		this->points[i] = points[i].ToVector3();
	}

}

void Curve::SetPoints(const std::vector<Vector3>& points)
{
	this->points = points;
}

Vector3 Curve::CalcPosition()
{
	Vector3 pos = 0;

	auto pointsSize = points.size();
	int powNum = static_cast<int>(pointsSize) - 1;
	float mulT = 1.0f;
	for (int i = 0; i < pointsSize - 1; i++)
	{
		float mulNum = 1.0f;
		if (i != 0)
		{
			mulNum = pointsSize - 1.0f;
		}

		float calcPowNum = 1.0f;
		for (int j = 0; j < powNum; j++)
		{
			calcPowNum *= (1.0f - t);
		}

		pos += mulNum * points[i] * calcPowNum * mulT;

		mulT *= t;
		powNum--;
	}

	pos += points[pointsSize - 1] * mulT;

	return pos;
}

Vector2 Curve::GetVector2Position()
{
	return CalcPosition().ToVector2();
}

Vector3 Curve::GetVector3Position()
{
	return CalcPosition();
}

void Curve::AddNumber(const float num)
{
	t += num;

	if (t >= 1.0f) 
	{
		if (isMaxNumberReset) t = 0.0f;
		else t = 1.0f;
	}
}
