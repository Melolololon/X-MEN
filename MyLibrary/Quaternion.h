#pragma once
#include"Vector.h"

namespace MelLib
{
	struct Vector2;
	struct Vector3;

	struct Quaternion
	{
		float w = 0.0f, x = 0.0f, y = 0.0f, z = 0.0f;

		Quaternion();
		Quaternion(const float w, const float x, const float y, const float z);
		Quaternion(const float w, const Vector3& v);
		Quaternion(const float w, const Vector2& v);
		
	
		Vector2 ToVector2()const;
		Vector3 ToVector3()const;


		static Quaternion ToQuaternion(const Vector2& v);
		static Quaternion ToQuaternion(const Vector3& v);

		Quaternion operator* (const Quaternion& q)const;
		void operator*= (const Quaternion& q);


		static Quaternion GetRotateQuaternion(const Vector3& pos, const Vector3& vector, const float angle);
		static Quaternion GetZXYRotateQuaternion(const Vector3& pos, const Vector3& angle);

		static Quaternion GetConjugatedQuaternion(const Vector3& pos, const Vector3& vector, const float angle);
		static Quaternion GetConjugatedQuaternion(Quaternion q);

		
	};
}

