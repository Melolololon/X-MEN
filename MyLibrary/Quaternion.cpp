#include "Quaternion.h"
#include<cmath>
#include"LibMath.h"

using namespace MelLib;
Quaternion::Quaternion(){}

Quaternion::Quaternion(const float w, const float x, const float y, const float z)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

Quaternion::Quaternion(const float w, const Vector3& v)
{
	this->w = w;
	x = v.x;
	y = v.y;
	z = v.z;
}

Quaternion::Quaternion(const float w, const Vector2& v)
{
	this->w = w;
	x = v.x;
	y = v.y;
	z = 0;
}

Vector2 MelLib::Quaternion::ToVector2()const
{
	return Vector2(x, y);
}

Vector3 MelLib::Quaternion::ToVector3()const
{
	return Vector3(x, y, z);
}

Quaternion MelLib::Quaternion::ToQuaternion(const Vector2& v)
{
	return Quaternion(1, v.x, v.y, 0);
}

Quaternion MelLib::Quaternion::ToQuaternion(const Vector3& v)
{
	return Quaternion(1, v.x, v.y, v.z);
}




Quaternion Quaternion::operator* (const Quaternion& q)const
{
	return 
	{
		-x * q.x - y * q.y - z * q.z + w * q.w,
		 x * q.w + y * q.z - z * q.y + w * q.x,
		-x * q.z + y * q.w + z * q.x + w * q.y,
		 x * q.y - y * q.x + z * q.w + w * q.z
	};

}

void Quaternion::operator*= (const Quaternion& q)
{
	Quaternion p(w, x, y, z);
	p = p * q;

	x = p.x;
	y = p.y;
	z = p.z;
	w = p.w;
}



Quaternion Quaternion::GetRotateQuaternion(const Vector3& pos, const Vector3& vector, const float angle)
{

	float rad = LibMath::AngleConversion(0, angle);
	Vector3 nVector = Vector3Normalize(vector);//正規化
	Quaternion p(pos.ToQuaternion());
	Quaternion q
	(
		std::cos(rad / 2),
		nVector.x * std::sin(rad / 2),
		nVector.y * std::sin(rad / 2),
		nVector.z * std::sin(rad / 2)
	);

	Quaternion r = GetConjugatedQuaternion(q);
	//(
	//	std::cos(rad / 2),
	//	-nVector.x * std::sin(rad / 2),
	//	-nVector.y * std::sin(rad / 2),
	//	-nVector.z * std::sin(rad / 2)
	//);
	return r * p * q;

}

Quaternion MelLib::Quaternion::GetZXYRotateQuaternion(const Vector3& pos, const Vector3& angle)
{
	//マイナスにすると、行列と同じように回転する
	Quaternion q = GetRotateQuaternion(pos, Vector3(0, 0, -1), angle.z);
	q = GetRotateQuaternion(q.ToVector3(), Vector3(-1, 0, 0), angle.x);
	q = GetRotateQuaternion(q.ToVector3(), Vector3(0, -1, 0), angle.y);
	return q;
}

Quaternion MelLib::Quaternion::GetConjugatedQuaternion(const Vector3& pos, const Vector3& vector, const float angle)
{
	Quaternion q = GetRotateQuaternion(pos, vector, angle);
	q.x *= -1;
	q.y *= -1;
	q.z *= -1;
	return q;
}

Quaternion MelLib::Quaternion::GetConjugatedQuaternion(Quaternion q)
{
	q.x *= -1;
	q.y *= -1;
	q.z *= -1;
	return q;
}
