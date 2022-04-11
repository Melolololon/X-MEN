#include"Vector.h"
#include<algorithm>
#include<tuple>

using namespace MelLib;

#pragma region Vector2

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(const float num)
{
	x = num;
	y = num;
}
Vector2::Vector2(const float x, const float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2(const DirectX::XMFLOAT2& f)
{
	x = f.x;
	y = f.y;
}

Vector2::Vector2(const DirectX::XMVECTOR& v)
{
	x = v.m128_f32[0];
	y = v.m128_f32[1];
}

Vector3 MelLib::Vector2::ToVector3()const
{
	return Vector3(x, y, 0);
}

Quaternion MelLib::Vector2::ToQuaternion()const
{
	return Quaternion(1, x, y, 0);
}

Vector2 MelLib::Vector2::ToVector2(const Vector3& v)
{
	return Vector2(v.x, v.y);
}

Vector2 MelLib::Vector2::ToVector2(const Quaternion& q)
{
	return Vector2(q.x, q.y);
}

Vector2 MelLib::Vector2::ToVector2(const DirectX::XMFLOAT2& f)
{
	return Vector2(f.x, f.y);
}

#pragma region ââéZéq



Vector2 MelLib::operator+(const Vector2& vector1, const Vector2& vector2)
{
	return { vector1.x + vector2.x, vector1.y + vector2.y };
}
Vector2 MelLib::operator-(const Vector2& vector1, const Vector2& vector2)
{
	return { vector1.x - vector2.x,vector1.y - vector2.y };
}
Vector2 MelLib::operator*(const Vector2& vector1, const Vector2& vector2)
{
	return { vector1.x * vector2.x, vector1.y * vector2.y };
}
Vector2 MelLib::operator/(const Vector2& vector1, const Vector2& vector2)
{
	return { vector1.x / vector2.x,vector1.y / vector2.y };
}


Vector2 MelLib::operator+(const Vector2& vector, const float f)
{
	return { vector.x + f,vector.y + f };
}
Vector2 MelLib::operator-(const Vector2& vector, const float f)
{
	return { vector.x - f, vector.y - f };
}
Vector2 MelLib::operator*(const Vector2& vector, const float f)
{
	return { vector.x * f, vector.y * f };
}
Vector2 MelLib::operator/(const Vector2& vector, const float f)
{
	return { vector.x / f,vector.y / f };
}

Vector2 MelLib::operator+(const float f, const Vector2& vector)
{
	return { f + vector.x ,f + vector.y };
}

Vector2 MelLib::operator-(const float f, const Vector2& vector)
{
	return { f - vector.x ,f - vector.y };
}

Vector2 MelLib::operator*(const float f, const Vector2& vector)
{
	return { f * vector.x ,f * vector.y };
}

Vector2 MelLib::operator/(const float f, const Vector2& vector)
{
	return { f / vector.x,f / vector.y };
}


void Vector2::operator+= (const Vector2& vector)
{
	x = x + vector.x;
	y = y + vector.y;
}

void Vector2::operator-= (const Vector2& vector)
{
	x = x - vector.x;
	y = y - vector.y;
}

void Vector2::operator*= (const Vector2& vector)
{
	x = x * vector.x;
	y = y * vector.y;
}

void Vector2::operator/= (const Vector2& vector)
{
	x = x / vector.x;
	y = y / vector.y;
}



bool Vector2::operator==(const Vector2& vector)
{
	if (x == vector.x &&
		y == vector.y)
		return true;
	return false;
}

bool Vector2::operator!=(const Vector2& vector)
{
	if (x == vector.x &&
		y == vector.y)
		return false;
	return true;
}

void Vector2::operator++()
{
	x++;
	y++;
}
void Vector2::operator--()
{
	x--;
	y--;
}

Vector2 Vector2::operator-()
{
	Vector2 v = Vector2(x, y);
	return v * -1;
}


#pragma endregion

#pragma region ä÷êî
float Vector2::Length()const
{
	return static_cast<float>(sqrt(x * x + y * y));
}

float Vector2::Length(const Vector2& vector)
{
	return vector.Length();
}

Vector2 Vector2::Normalize()const
{
	if (x == 0 && y == 0)return 0;
	float length = Length(Vector2(x, y));
	return Vector2(x / length, y / length);
}

Vector2 Vector2::Normalize(const Vector2& vector)
{
	return vector.Normalize();
}

float Vector2::Dot(const Vector2& vector)const
{
	return x * vector.x + y * vector.y;
}
float Vector2::Dot(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.Dot(vector2);
}


float Vector2::Cross(const Vector2& vector)const
{
	return x * vector.y - y * vector.x;
}
float Vector2::Cross(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.Cross(vector2);
}


Vector2 Vector2::Abs()const
{
	Vector2 returnVector = Vector2(x, y);
	if (returnVector.x < 0)returnVector.x *= -1;
	if (returnVector.y < 0)returnVector.y *= -1;
	return returnVector;
}
Vector2 Vector2::Abs(const Vector2& vector)
{
	return vector.Abs();
}

Vector2 MelLib::Vector2Normalize(const Vector2& vector)
{
	if (vector.x == 0 && vector.y == 0)return { 0,0 };
	float length = static_cast<float>(sqrt(vector.x * vector.x + vector.y * vector.y));
	return{ vector.x / length ,vector.y / length };
}


/// <summary>
/// ì‡êœÇãÅÇﬂÇ‹Ç∑
/// </summary>
/// <param name="vector1"></param>
/// <param name="vector2"></param>
/// <returns></returns>
float MelLib::Vector2Dot(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y;
}

float MelLib::Vector2Cross(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}
#pragma endregion


#pragma endregion




#pragma region Vector3

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}


Vector3::Vector3(const float num)
{
	x = num;
	y = num;
	z = num;
}

Vector3::Vector3(const float x, const float y, const float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(const DirectX::XMFLOAT3& f)
{
	x = f.x;
	y = f.y;
	z = f.z;
}


Vector3::Vector3(const DirectX::XMVECTOR& v)
{
	x = v.m128_f32[0];
	y = v.m128_f32[1];
	z = v.m128_f32[2];

}

Vector2 Vector3::ToVector2()const { return Vector2(x, y); }
Quaternion Vector3::ToQuaternion()const { return Quaternion(1, x, y, z); }

Vector3 MelLib::Vector3::ToVector3(const Vector2& v)
{
	return Vector3(v.x, v.y, 0);
}

Vector3 MelLib::Vector3::ToVector3(const Quaternion& q)
{
	return Vector3(q.x, q.y, q.z);
}

Vector3 MelLib::Vector3::ToVector3(const DirectX::XMFLOAT3& f)
{
	return Vector3(f.x, f.y, f.z);
}

#pragma region ââéZéq


Vector3 MelLib::operator+(const Vector3& vector1, const Vector3& vector2)
{
	return { vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z };
}
Vector3 MelLib::operator-(const Vector3& vector1, const Vector3& vector2)
{
	return { vector1.x - vector2.x,vector1.y - vector2.y, vector1.z - vector2.z };
}
Vector3 MelLib::operator*(const Vector3& vector1, const Vector3& vector2)
{
	return { vector1.x * vector2.x, vector1.y * vector2.y, vector1.z * vector2.z };
}
Vector3 MelLib::operator/(const Vector3& vector1, const Vector3& vector2)
{
	return { vector1.x / vector2.x, vector1.y / vector2.y, vector1.z / vector2.z };
}

Vector3 MelLib::operator+(const Vector3& vector, const float f)
{
	return { vector.x + f, vector.y + f,vector.z + f };
}
Vector3 MelLib::operator-(const Vector3& vector, const float f)
{
	return { vector.x - f, vector.y - f, vector.z - f };
}
Vector3 MelLib::operator*(const Vector3& vector, const float f)
{
	return { vector.x * f,vector.y * f, vector.z * f };
}
Vector3 MelLib::operator/(const Vector3& vector, const float f)
{
	return { vector.x / f, vector.y / f, vector.z / f };
}


Vector3 MelLib::operator+(const float f, const Vector3& vector)
{
	return { f + vector.x,f + vector.y,f + vector.z };
}
Vector3 MelLib::operator-(const float f, const Vector3& vector)
{
	return { f - vector.x,f - vector.y,f - vector.z };
}
Vector3 MelLib::operator*(const float f, const Vector3& vector)
{
	return { f * vector.x,f * vector.y,f * vector.z };
}
Vector3 MelLib::operator/(const float f, const Vector3& vector)
{
	return { f / vector.x,f / vector.y,f / vector.z };
}


void Vector3::operator+= (const Vector3& vector)
{
	x = x + vector.x;
	y = y + vector.y;
	z = z + vector.z;
}

void Vector3::operator-= (const Vector3& vector)
{
	x = x - vector.x;
	y = y - vector.y;
	z = z - vector.z;
}

void Vector3::operator*= (const Vector3& vector)
{
	x = x * vector.x;
	y = y * vector.y;
	z = z * vector.z;
}

void Vector3::operator/= (const Vector3& vector)
{
	x = x / vector.x;
	y = y / vector.y;
	z = z / vector.z;
}


bool Vector3::operator==(const Vector3& vector)const
{
	if (x == vector.x &&
		y == vector.y &&
		z == vector.z)
		return true;
	return false;
}

bool Vector3::operator!=(const Vector3& vector)const
{
	if (x == vector.x &&
		y == vector.y &&
		z == vector.z)
		return false;
	return true;
}


void Vector3::operator++()
{
	x++;
	y++;
	z++;
}
void Vector3::operator--()
{
	x--;
	y--;
	z--;
}

Vector3 Vector3::operator-()
{
	Vector3 vector = Vector3(x, y, z);
	return vector * -1;
}

#pragma endregion

#pragma region ä÷êî
float Vector3::Length()const
{
	return static_cast<float>(sqrt(x * x + y * y + z * z));
}
float Vector3::Length(const Vector3& vector) 
{
	return vector.Length();
}

Vector3 Vector3::Normalize() const
{
	if (x == 0 && y == 0 && z == 0)return 0;
	float length = Vector3(x, y, z).Length();
	return Vector3(x / length ,y / length ,z / length );
}

Vector3 Vector3::Normalize(const Vector3& vector) 
{
	return vector.Normalize();
}

float Vector3::Dot(const Vector3& vector)const
{
	return x * vector.x + y * vector.y + z * vector.z;
}
float Vector3::Dot(const Vector3& vector1, const Vector3& vector2)
{
	return vector1.Dot(vector2);
}

Vector3 Vector3::Cross(const Vector3& vector)const
{
	Vector3 cross;
	cross.x = y * vector.z - z * vector.y;
	cross.y = z * vector.x - x * vector.z;
	cross.z = x * vector.y - y * vector.x;

	return cross;
}


Vector3 Vector3::Cross(const Vector3& vector1, const Vector3& vector2)
{
	return vector1.Cross(vector2);
}

Vector3 Vector3::Abs()const
{
	Vector3 returnVector = Vector3(x, y, z);
	if (returnVector.x < 0)returnVector.x *= -1;
	if (returnVector.y < 0)returnVector.y *= -1;
	if (returnVector.z < 0)returnVector.z *= -1;
	return returnVector;
}
Vector3 Vector3::Abs(const Vector3& vector)
{
	return vector.Abs();
}

std::vector<Vector3> MelLib::Vector3::Sort(const std::vector<Vector3>& vectors, const Vector3& standardVec, const SortType type)
{
	size_t vecNum = vectors.size();
	std::vector<std::tuple<Vector3,float,int>>sortVectors(vecNum);
	
	for (int i = 0; i < vecNum; i++)
	{	
		Vector3 diff = (standardVec - vectors[i]).Abs();
		sortVectors[i] = std::make_tuple(diff, diff.Length(),i);
		//std::get<1>(v)
	}

	if (type == SortType::ASCENDING)
	{
		std::sort(sortVectors.begin(), sortVectors.end(),
			[](const std::tuple<Vector3, float, int>& t1, const std::tuple<Vector3, float, int>& t2)
		{
			return std::get<1>(t1) < std::get<1>(t2);
		});
	}
	else
	{
		std::sort(sortVectors.begin(), sortVectors.end(),
			[](const std::tuple<Vector3, float, int>& t1, const std::tuple<Vector3, float, int>& t2)
		{
			return std::get<1>(t1) > std::get<1>(t2);
		});
	}

	std::vector<Vector3>returnVectors(vecNum);
	for (int i = 0; i < vecNum; i++)
	{
		returnVectors[i] = vectors[std::get<2>(sortVectors[i])];
	}

	return returnVectors;

	//auto vecSort = [&type,&standardVec](const Vector3& v1, const Vector3& v2)
	//{
	//	Vector3 diff1 = (standardVec - v1).Abs();
	//	float sum1 = diff1.x + diff1.y + diff1.z;

	//	Vector3 diff2 = (standardVec - v2).Abs();
	//	float sum2 = diff2.x + diff2.y + diff2.z;

	//	if (type == SortType::ASCENDING)return sum1 < sum2;
	//	return sum1 > sum2;

	//};

}


/// <summary>
/// ê≥ãKâªÇµÇ‹Ç∑
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
Vector3 MelLib::Vector3Normalize(const Vector3& vector)
{
	if (vector.x == 0 && vector.y == 0 && vector.z == 0)return { 0,0,0 };
	float length = (float)(sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z));
	return{ vector.x / length ,vector.y / length ,vector.z / length };
}

/// <summary>
/// äOêœÇãÅÇﬂÇ‹Ç∑
/// </summary>
/// <param name="vector1"></param>
/// <param name="vector2"></param>
/// <returns></returns>
Vector3 MelLib::Vector3Cross(const Vector3& vector1, const Vector3& vector2)
{
	Vector3 cross;
	cross.x = vector1.y * vector2.z - vector1.z * vector2.y;
	cross.y = vector1.z * vector2.x - vector1.x * vector2.z;
	cross.z = vector1.x * vector2.y - vector1.y * vector2.x;

	return cross;
}
/// <summary>
/// ì‡êœÇãÅÇﬂÇ‹Ç∑
/// </summary>
/// <param name="vector1"></param>
/// <param name="vector2"></param>
/// <returns></returns>
float MelLib::Vector3Dot(const Vector3& vector1, const Vector3& vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}
#pragma endregion

#pragma endregion


