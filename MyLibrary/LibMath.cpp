#include"LibMath.h"
#include"DirectInput.h"

#include"Interpolation.h"

using namespace MelLib;



float LibMath::MultipleClamp
(
	const float num,
	const float multiple,
	const MultipleClampType type
)
{
	if (multiple == 0.0f
		|| num == 0.0f)return 0.0f;

	//smallNumとbigNum確認用の変数
	float checkNum = 0.0f;
	//numより小さいかつ一番近い数値
	float smallNum = 0.0f;
	//numより大きいかつ一番近い数値
	float bigNum = 0.0f;

	bool checkSmall = false;
	bool checkBig = false;

	while (1)
	{
		if (num >= checkNum)
		{
			smallNum = checkNum;
			checkSmall = true;
		}
		else
		{
			bigNum = checkNum;
			checkBig = true;
		}

		if (checkSmall && checkBig)break;

		//符号が違う場合は減らす
		if (num >= 0 && multiple < 0
			|| num < 0 && multiple >= 0)
		{
			checkNum -= multiple;
		}
		else
		{
			checkNum += multiple;
		}
	}

	if (type == MultipleClampType::CLAMP_TYPE_BIG)return bigNum;
	else if (type == MultipleClampType::CLAMP_TYPE_SMALL)return smallNum;

	float bigDif = bigNum - num;
	float smallDif = num - smallNum;


	//差が小さいほう(近いほう)をリターン
	if (smallDif > bigDif)
	{
		return bigNum;
	}
	else if (smallDif < bigDif)
	{
		return smallNum;
	}
	else//同じだった場合、タイプに従う
	{
		if (type == MultipleClampType::CLAMP_TYPE_NEAR_BIG)return bigNum;
		return smallNum;
	}
}




int LibMath::Factorial(const int num)
{
	int returnNum = num;
	for(int i = num;i > -1;i--)
	{
		returnNum *= i;
	}
	return returnNum;
}

int LibMath::Pow(const float num, const int index)
{
	
	float returnNum = 1.0f;
	if(index >= 0)
	{
		for(int i = 0; i < index;i++)
		{
			returnNum *= num;
		}
	}
	else
	{
		for (int i = 0; i < abs(index); i++)
		{
			returnNum /= num;
		}
	}

	return returnNum;
	
}

float LibMath::AngleConversion(int paterrn, float angle)
{
	if (paterrn == 0)
	{
		return (float)(angle * (M_PI / 180));
	}

	return (float)(angle * (180 / M_PI));

}




bool LibMath::Difference(const float num1, const float num2, const float difference)
{
	float sum = num1 - num2;
	float diff = abs(sum);//差を求める

		//差が指定した値以内だったらtrue
	if (difference >= diff)
		return true;

	return false;

}
bool LibMath::AngleDifference(const float angle1, const float angle2, const float difference)
{
	
	float ang1 = angle1;
	if (angle1 < 0) ang1 = 360 - (abs(angle1)); 
	if (angle1 > 360)ang1 = angle1 - 360;

	float ang2 = angle2;
	if (angle2 < 0) ang2 = 360 - (abs(angle2));
	if (angle2 > 360)ang2 = angle2 - 360;

	float sum = ang2 - ang1;
	float diff = abs(sum);//差を求める

	//通常の判定
	bool flag1 = difference >= diff;
	
	//小さいほうを入れる
	int smallNum = min(ang1, ang2);
	int bigNum = max(ang1, ang2);
	sum = bigNum - (smallNum + 360.0f);
	diff = abs(sum);
	bool flag2 = difference >= diff;

	if (flag1 || flag2)
		return true;

	return false;
}


float LibMath::GetFloatPI()
{
	return static_cast<float>(M_PI);
	//return 0;
}

double LibMath::GetDoublePI()
{
	return M_PI;
	//return 0;
}

#pragma region ベクトル


#pragma region vector2

float LibMath::CalcDistance2D(const Vector2& pos1, const Vector2& pos2)
{
	return sqrt
	(
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y)
	);
}

Vector2 MelLib::LibMath::CalcCenterPosition2D(const Vector2& pos1, const Vector2& pos2)
{
	return Vector2(pos1 + pos2) / 2;
}

Vector2 LibMath::OtherVector2(const Vector2& vec1, const Vector2& vec2)
{
	return (vec2 - vec1).Normalize();
}


char LibMath::PointLeftRightCheck(const Vector2& vector, const Vector2& point)
{
	float num = Vector2Cross(vector, point);

	if (num > 0)return 1;
	if (num < 0)return -1;
	return 0;
}

char MelLib::LibMath::PointPlaneFrontBackCheck(const Vector3& point, const PlaneData& plane)
{

	//原点から法線方向に距離分動いた座標を求める。
	Vector3 disPos = plane.GetPosition();

	if (Vector3::Dot(point - disPos, plane.GetNormal()) > 0)return 1;
	else if (Vector3::Dot(point - disPos, plane.GetNormal()) < 0)return -1;
	return 0;
}

char MelLib::LibMath::PointBoardFrontBackCheck(const Vector3& point, const BoardData& board)
{

	//原点から法線方向に距離分動いた座標を求める。
	Vector3 disPos = board.GetPosition();

	if (Vector3::Dot(point - disPos, board.GetNormal()) > 0)return 1;
	else if (Vector3::Dot(point - disPos, board.GetNormal()) < 0)return -1;
	return 0;
}


float LibMath::TwoVector2Angle(const Vector2& v1, const Vector2& v2)
{

	float f = Vector2Dot(v1, Vector2Normalize( v2));
	f = std::acos(f);
	f = AngleConversion(1, f);

	Vector3 v = Vector3Cross({ v1.x,v1.y,0 }, { v2.x,v2.y,0 });
	if (v.z < 0)f = 360 - f;

	return f;
}

float LibMath::Vector2ToAngle(const Vector2& v, const bool v3)
{
	float f = 0.0f;
	if (v3)
		f = TwoVector2Angle({ 1,0 }, v);
	else
		f = TwoVector2Angle({ 1,0 }, { v.x,-v.y });
	return f;
}


Vector2 LibMath::AngleToVector2(const float& angle, const bool v3)
{
	Quaternion q = Quaternion::GetRotateQuaternion({ 1,0,0 }, { 0,0,-1 }, angle);

	if (v3)return { q.x,q.y };
	return { q.x,-q.y };
}


Vector2 LibMath::RotateVector2(const Vector2& v, const float angle)
{
	Quaternion q = Quaternion::GetRotateQuaternion({ v.x,v.y,0 }, { 0,0,1 }, angle);
	return { q.x,q.y };
}
Vector2 MelLib::LibMath::RotateVector2Box(const Vector2& v, const float angle)
{
	float rotAngle = angle;
	int startNum = 0;

	while(1)
	{
		if(rotAngle >= 90 && angle > 0)
		{
			rotAngle -= 90;
			startNum++;

			if (startNum == 4)startNum = 0;
			
		}
		else if(rotAngle <= -90 && angle < 0)
		{
			rotAngle += 90;
			startNum--;

			if (startNum == -1)startNum = 3;
		}
		else
		{
			break;
		}
	}


	Vector2 startPos, endPos;
	
	
	if(startNum == 0)
	{
		startPos = v;

		if(angle > 0)endPos = RotateVector2(v, 90.0f);
		else endPos = RotateVector2(v, -90.0f);
	}
	else if(startNum == 1)
	{
		startPos = RotateVector2(v, 90.0f);
		endPos = RotateVector2(v, 180.0f);

		endPos = RotateVector2(v, 180.0f);
	
	}
	else if (startNum == 2)
	{
		startPos = RotateVector2(v, 180.0f);
		if (angle > 0)endPos = RotateVector2(v, 270.0f);
		else endPos = RotateVector2(v, -270.0f);
	}
	else if (startNum == 3)
	{
		startPos = RotateVector2(v, 270.0f);
		endPos = v;
	}

	return  Interpolation::Lerp(startPos, endPos, rotAngle / 90.0f);

}
#pragma endregion

#pragma region Vector3

Vector3 LibMath::CalcNormal(const Vector3& pos1, const Vector3& pos2, const Vector3& pos3)
{
	return Vector3::Normalize(Vector3::Cross(pos2 - pos1, pos3 - pos1));
}

float LibMath::CalcDistance3D(const Vector3& pos1, const Vector3& pos2)
{
	return sqrt
	(
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y) +
		(pos1.z - pos2.z) * (pos1.z - pos2.z)
	);
}

Vector3 MelLib::LibMath::CalcCenterPosition3D(const Vector3& pos1, const Vector3& pos2)
{
	return (pos1 + pos2) / 2;
}

Vector3 LibMath::OtherVector3(const Vector3& vec1, const Vector3& vec2)
{
	Vector3 vec;
	vec = vec2 - vec1;
	return Vector3Normalize(vec);
}

Vector3 LibMath::RotateVector3(const Vector3& rotateV, const Vector3& vec, const float angle)
{
	Quaternion q = Quaternion::GetRotateQuaternion(rotateV, vec, angle);
	return { q.x, q.y, q.z };
}

Vector3 MelLib::LibMath::RotateZXYVector3(const Vector3& rotateV, const Vector3& angle)
{
	return Quaternion::GetZXYRotateQuaternion(rotateV,angle).ToVector3();
}



#pragma endregion


#pragma endregion


#pragma region 当たり判定
#pragma region 2D

bool LibMath::RectAndRectCollision
(
	const Vector2& pos1,
	const Vector2& size1,
	const Vector2& pos2,
	const Vector2& size2
)
{
	if (size2.x + pos2.x > pos1.x &&
		pos2.x < size1.x + pos1.x &&
		pos2.y < size1.y + pos1.y &&
		size2.y + pos2.y > pos1.y) return true;
	return false;
}

bool LibMath::CircleAndCircleCollision
(
	const Vector2& centerPosition1, 
	const float r1,
	const Vector2& centerPosition2, 
	const float r2
)//円判定
{
	if ((centerPosition1.x - centerPosition2.x) *
		(centerPosition1.x - centerPosition2.x) +
		(centerPosition1.y - centerPosition2.y) *
		(centerPosition1.y - centerPosition2.y) <=
		(r1 + r2) * (r1 + r2))
	{
		return true;
	}
	return false;
}

bool LibMath::CircleAndLineSegmentCollision
(
	const Vector2& spherePos,
	const float r,
	const Vector2& linePos1,
	const Vector2& linePos2,
	Vector2* nearPos,
	SegmentHitPlace* isHit
)
{

	if (isHit)*isHit = SegmentHitPlace::LS_HIT_POSITION_NOT_HIT;

	//線の端から端へのベクトル
	Vector2 lineVector = linePos2 - linePos1;
	//線の座標1から円の中心
	Vector2 linePos1ToSphere = spherePos - linePos1;
	//円と線の最短距離を求める
	float distance = abs(Vector2::Cross(lineVector, linePos1ToSphere) / Vector2::Length(lineVector));


	//円と線分の最近点を求める
	//これ関数化する
	if (nearPos)
	{
		//linePos1から最近点の距離を求める
		float linePos1ToNearPosDis = Vector2::Dot(Vector2::Normalize(lineVector), linePos1ToSphere);

		*nearPos = linePos1 + (Vector2::Normalize(lineVector) * linePos1ToNearPosDis);
	}


	//距離のほうが大きかったら計算終了
	if (distance > r)return false;

	//線の座標2から円の中心
	Vector2 linePos2ToSphere = spherePos - linePos2;
	//正の数かどうかを判別するフラグ
	bool linePos1ToSpherePositive = false;
	bool linePos2ToSpherePositive = false;

	//内積を求める
	float linePos1ToSphereDot = Vector2::Dot(lineVector, linePos1ToSphere);
	float linePos2ToSphereDot = Vector2::Dot(lineVector, linePos2ToSphere);

	//正か負か確認
	if (linePos1ToSphereDot >= 0)
		linePos1ToSpherePositive = true;
	if (linePos2ToSphereDot >= 0)
		linePos2ToSpherePositive = true;

	//どちらも正または負だったら入る
	if (linePos1ToSpherePositive
		&& linePos2ToSpherePositive
		|| !linePos1ToSpherePositive
		&& !linePos2ToSpherePositive)
	{
		//大きさを求める
		float linePos1ToSphereLength = Vector2::Length(linePos1ToSphere);
		float linePos2ToSphereLength = Vector2::Length(linePos2ToSphere);

		//半径より大きかったら当たってない
		if (linePos1ToSphereLength > r
			&& linePos2ToSphereLength > r) 
		{
			return false;
		}

		if (isHit)*isHit = SegmentHitPlace::LS_HIT_POSITION_LE_START_END;
	}
	else
	{
		if (isHit)*isHit = SegmentHitPlace::LS_HIT_POSITION_LE_LINE;
	}

	return true;
}


#pragma endregion

#pragma region 3D

bool LibMath::SphereAndSphereCollision
(
	const Vector3& centerPosition1,
	const float r1, 
	const Vector3& centerPosition2,
	const float r2
)
{
	if ((centerPosition2.x - centerPosition1.x) *
		(centerPosition2.x - centerPosition1.x) +
		(centerPosition2.y - centerPosition1.y) *
		(centerPosition2.y - centerPosition1.y) +
		(centerPosition2.z - centerPosition1.z) *
		(centerPosition2.z - centerPosition1.z) <=
		(r1 + r2) * (r1 + r2))
	{
		return true;
	}
	return false;
}

bool LibMath::SphereAndPlaneCollision
(
	const Vector3& spherePos,
	const float r,
	const Vector3& normal,
	const float planeDist, 
	Vector3* hitPos
)
{
	//原点から球の距離
	float sphereDist = Vector3Dot(spherePos, normal);

	float dict = sphereDist - planeDist;
	if (fabsf(dict) > r)return false;

	if (hitPos) *hitPos = normal * -dict + spherePos;
	return true;
}

bool LibMath::SphereAndTryangleCollision
(
	const Vector3& spherePos,
	const float r,
	const Vector3& triPos1,
	const Vector3& triPos2,
	const Vector3& triPos3,
	const Vector3& normal,
	Vector3* hitPos
)
{
	//最近接点
	Vector3 closest = { 0,0,0 };
	//内積
	float dot1, dot2;
	//所属エリア
	int area = 0;

#pragma region 領域チェック1

	if (area == 0)
	{
		//tryPos1、tryPos2のベクトルと、tryPos1、spherePosのベクトルの内積を求める
		dot1 = Vector3Dot(triPos2 - triPos1, spherePos - triPos1);

		//tryPos1、tryPos3のベクトルと、tryPos1、spherePosのベクトルの内積を求める
		dot2 = Vector3Dot(triPos3 - triPos1, spherePos - triPos1);

		//dot1とdot2の両方がマイナスなら、領域1にある
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 1;
			closest = triPos1;
		}
	}
#pragma endregion

#pragma region 領域チェック2

	if (area == 0)
	{
		dot1 = Vector3Dot(triPos1 - triPos2, spherePos - triPos2);

		dot2 = Vector3Dot(triPos3 - triPos2, spherePos - triPos2);

		//dot1とdot2の両方がマイナスなら、領域1にある
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 2;
			closest = triPos2;
		}
	}
#pragma endregion

#pragma region 領域チェック4

	if (area == 0)
	{
		dot1 = Vector3Dot(triPos2 - triPos3, spherePos - triPos3);

		dot2 = Vector3Dot(triPos1 - triPos3, spherePos - triPos3);

		//dot1とdot2の両方がマイナスなら、領域1にある
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 4;
			closest = triPos3;
		}
	}
#pragma endregion


#pragma region 領域チェック3

	if (area == 0)
	{
		dot1 = Vector3Dot(triPos2 - triPos1, spherePos - triPos1);
		dot2 = Vector3Dot(triPos2 - triPos1, spherePos - triPos2);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (Vector3Dot(triPos2 - triPos1, spherePos - triPos1) * Vector3Dot(triPos3 - triPos1, spherePos - triPos2) -
				Vector3Dot(triPos2 - triPos1, spherePos - triPos2) * Vector3Dot(triPos3 - triPos1, spherePos - triPos1) <= 0)
			{
				area = 3;
				float num =
					Vector3Dot(triPos2 - triPos1, spherePos - triPos1) /
					(Vector3Dot(triPos2 - triPos1, spherePos - triPos1) - Vector3Dot(triPos2 - triPos1, spherePos - triPos2));
				closest.x = triPos1.x + num * (triPos2.x - triPos1.x);
				closest.y = triPos1.y + num * (triPos2.y - triPos1.y);
				closest.z = triPos1.z + num * (triPos2.z - triPos1.z);
			}


		}
	}
#pragma endregion

#pragma region 領域チェック5
	if (area == 0)
	{
		dot1 = Vector3Dot(triPos3 - triPos2, spherePos - triPos2);
		dot2 = Vector3Dot(triPos3 - triPos2, spherePos - triPos3);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (Vector3Dot(triPos3 - triPos2, spherePos - triPos2) * Vector3Dot(triPos1 - triPos2, spherePos - triPos3) -
				Vector3Dot(triPos3 - triPos2, spherePos - triPos3) * Vector3Dot(triPos1 - triPos2, spherePos - triPos2) <= 0)
			{

				area = 5;

				float num =
					Vector3Dot(triPos3 - triPos2, spherePos - triPos2) /
					(Vector3Dot(triPos3 - triPos2, spherePos - triPos2) - Vector3Dot(triPos3 - triPos2, spherePos - triPos3));
				closest.x = triPos2.x + num * (triPos3.x - triPos2.x);
				closest.y = triPos2.y + num * (triPos3.y - triPos2.y);
				closest.z = triPos2.z + num * (triPos3.z - triPos2.z);

			}
		}
	}
#pragma endregion

#pragma region 領域チェック6
	if (area == 0)
	{
		dot1 = Vector3Dot(triPos1 - triPos3, spherePos - triPos3);
		dot2 = Vector3Dot(triPos1 - triPos3, spherePos - triPos1);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (Vector3Dot(triPos1 - triPos3, spherePos - triPos3) * Vector3Dot(triPos2 - triPos3, spherePos - triPos1) -
				Vector3Dot(triPos1 - triPos3, spherePos - triPos1) * Vector3Dot(triPos2 - triPos3, spherePos - triPos3) <= 0)
			{
				area = 6;

				float num =
					Vector3Dot(triPos1 - triPos3, spherePos - triPos3) /
					(Vector3Dot(triPos1 - triPos3, spherePos - triPos3) - Vector3Dot(triPos1 - triPos3, spherePos - triPos1));
				closest.x = triPos3.x + num * (triPos1.x - triPos3.x);
				closest.y = triPos3.y + num * (triPos1.y - triPos3.y);
				closest.z = triPos3.z + num * (triPos1.z - triPos3.z);

			}


		}
	}
#pragma endregion


#pragma region 領域チェック7
	if (area == 0)
	{
		float va =
			Vector3Dot(triPos2 - triPos1, spherePos - triPos2) * Vector3Dot(triPos3 - triPos1, spherePos - triPos3) -
			Vector3Dot(triPos2 - triPos1, spherePos - triPos3) * Vector3Dot(triPos3 - triPos1, spherePos - triPos2);

		float vb =
			Vector3Dot(triPos2 - triPos1, spherePos - triPos3) * Vector3Dot(triPos3 - triPos1, spherePos - triPos1) -
			Vector3Dot(triPos2 - triPos1, spherePos - triPos1) * Vector3Dot(triPos3 - triPos1, spherePos - triPos3);

		float vc =
			Vector3Dot(triPos2 - triPos1, spherePos - triPos1) * Vector3Dot(triPos3 - triPos1, spherePos - triPos2) -
			Vector3Dot(triPos2 - triPos1, spherePos - triPos2) * Vector3Dot(triPos3 - triPos1, spherePos - triPos1);

		float denom = 1.0f / (va + vb + vc);
		float v = vb * denom;
		float w = vc * denom;
		closest.x = triPos1.x + (triPos2.x - triPos1.x) * v + (triPos3.x - triPos1.x) * w;
		closest.y = triPos1.y + (triPos2.y - triPos1.y) * v + (triPos3.y - triPos1.y) * w;
		closest.z = triPos1.z + (triPos2.z - triPos1.z) * v + (triPos3.z - triPos1.z) * w;

		area = 7;
	}
#pragma endregion


	//最接近点 - 球の中心
	Vector3 v = closest - spherePos;
	float f = Vector3Dot(v, v);
	if (f > r * r) return false;

	if (hitPos)*hitPos = closest;
	return true;

}

bool LibMath::SphereAndLineSegmentCollision
(
	const Vector3& spherePos,
	const float r,
	const Vector3& linePos1,
	const Vector3& linePos2
)
{
	return false;
}



bool LibMath::SphereAndBoxCollision
(
	const Vector3& spherePos,
	const float& r,
	const Vector3& centerPos,
	const Vector3& size,
	BoxHitDirection* direction
)
{
	float dir2 = 0.0f;

	Vector3 minPos = centerPos - size / 2;
	Vector3 maxPos = centerPos + size / 2;

	//x
	if (spherePos.x < minPos.x)
		dir2 += (minPos.x - spherePos.x) * (minPos.x - spherePos.x);
	if (spherePos.x > maxPos.x)
		dir2 += (spherePos.x - maxPos.x) * (spherePos.x - maxPos.x);

	//y
	if (spherePos.y < minPos.y)
		dir2 += (minPos.y - spherePos.y) * (minPos.y - spherePos.y);
	if (spherePos.y > maxPos.y)
		dir2 += (spherePos.y - maxPos.y) * (spherePos.y - maxPos.y);

	//z
	if (spherePos.z < minPos.z)
		dir2 += (minPos.z - spherePos.z) * (minPos.z - spherePos.z);
	if (spherePos.z > maxPos.z)
		dir2 += (spherePos.z - maxPos.z) * (spherePos.z - maxPos.z);

	bool flag = dir2 < r * r;

	//ボックスのどこに当たったかを返す
	if (direction)
	{

		if (!flag)
		{
			*direction = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			return flag;
		}

		//1 Xが多い
		//2 Yが多い
		//3 Zが多い
		char top = 0;
		//ボックスへのベクトル
		Vector3 sphereToVector = centerPos - spherePos;

		if (abs(sphereToVector.x) > abs(sphereToVector.y) &&
			abs(sphereToVector.x) > size.x / 2)
		{
			top = 1;
			if (abs(sphereToVector.z) > abs(sphereToVector.x) &&
				abs(sphereToVector.z) > size.z / 2)
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(sphereToVector.z) > abs(sphereToVector.y) &&
				abs(sphereToVector.z) > size.z / 2)
				top = 3;
		}

		if (top == 1)
		{
			if (sphereToVector.x >= 0)
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			else
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
		}
		if (top == 2)
		{
			if (sphereToVector.y >= 0)
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			else
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_UP;
		}
		if (top == 3)
		{
			if (sphereToVector.z >= 0)
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			else
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
		}
	}

	return flag;
}

bool LibMath::BoxAndBoxCollision
(
	const Vector3& centerPos1,
	const Vector3& size1,
	const Vector3& centerPos2,
	const Vector3& size2,
	BoxHitDirection* direction1,
	BoxHitDirection* direction2
)
{
	Vector3 minPos1 = centerPos1 - size1 / 2;
	Vector3 maxPos1 = centerPos1 + size1 / 2;
	Vector3 minPos2 = centerPos2 - size2 / 2;
	Vector3 maxPos2 = centerPos2 + size2 / 2;
	bool isHit = false;

	//1か2のminかmaxがXYZ全部相手の範囲内だったら当たってる
	if (minPos1.x >= minPos2.x &&
		minPos1.x < maxPos2.x ||
		maxPos1.x >= minPos2.x &&
		maxPos1.x < maxPos2.x ||
		minPos2.x >= minPos1.x &&
		minPos2.x < maxPos1.x ||
		maxPos2.x >= minPos1.x &&
		maxPos2.x < maxPos1.x)
	{

		if (minPos1.y >= minPos2.y &&
			minPos1.y < maxPos2.y ||
			maxPos1.y >= minPos2.y &&
			maxPos1.y < maxPos2.y ||
			minPos2.y >= minPos1.y &&
			minPos2.y < maxPos1.y ||
			maxPos2.y >= minPos1.y &&
			maxPos2.y < maxPos1.y)
		{
			if (minPos1.z >= minPos2.z &&
				minPos1.z < maxPos2.z ||
				maxPos1.z >= minPos2.z &&
				maxPos1.z < maxPos2.z ||
				minPos2.z >= minPos1.z &&
				minPos2.z < maxPos1.z ||
				maxPos2.z >= minPos1.z &&
				maxPos2.z < maxPos1.z)
				isHit = true;

		}
	}
		
	if (direction1 || direction2)
	{

		if (!isHit)
		{
			if(direction1)
			*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			if(direction2)
			*direction2 = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			return isHit;
		}

		//1 Xが多い
		//2 Yが多い
		//3 Zが多い
		char top = 0;
		//対象へのベクトル
		Vector3 targetToVector = centerPos2 - centerPos1;

		if (abs(targetToVector.x) > abs(targetToVector.y) &&
			abs(targetToVector.x) > size2.x / 2)
		{
			top = 1;
			if (abs(targetToVector.z) > abs(targetToVector.x) &&
				abs(targetToVector.z) > size2.z / 2)
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(targetToVector.z) > abs(targetToVector.y) &&
				abs(targetToVector.z) > size2.z / 2)
				top = 3;
		}

		if (top == 1)
		{
			if (targetToVector.x >= 0)
			{
				if (direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
				if (direction2)
					*direction2 = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			}
			else 
			{
				if (direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
				if (direction2)
					*direction2 = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
			}
		}
		if (top == 2)
		{
			if (targetToVector.y >= 0)
			{
				if (direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_UP;
				if (direction2)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			}
			else 
			{
				if (direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
				if (direction2)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_UP;
			}
		}
		if (top == 3)
		{
			if (targetToVector.z >= 0) 
			{
				if(direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
				if (direction2)
					*direction2 = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			}
			else 
			{
				if (direction1)
					*direction1 = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
				if (direction2)
					*direction2 = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
			}
		}
	}

	return isHit;
}

bool LibMath::LineSegmentAndBoardCollision
(
	const Vector3& linePos1,
	const Vector3& linePos2,
	const Vector3& normal,
	const Vector3& pointPos,
	const std::vector<Vector3>&vertexPoint,
	Vector3* crossPosition
)
{

	Vector3 v1;
	Vector3 v2;

	v1 = linePos1 - pointPos;
	v2 = linePos2 - pointPos;

	//線が板ポリと並行ではないかを調べる(平行だったらreturn)
	if (Vector3Dot(v1, normal) * Vector3Dot(v2, normal) > 0)
		return false;


	//ここにポリゴンの範囲内かどうかの処理を書く

	//線の端 - ポリゴンの角
	v1 = linePos1 - vertexPoint[0];
	v2 = linePos2 - vertexPoint[0];
	/*v1 = normalize(v1);
	v2 = normalize(v2);*/


	//ここがおかしい?

	////線の端から板ポリまでの距離を求める
	//float d = dot(normal,pointPos);
	//float kyori1;//平面から、linePos1までの距離
	//kyori1 = 
	//	abs(normal.x * linePos1.x + normal.y * linePos1.y +normal.z * linePos1.z + d) / 
	//	sqrt(normal.x* normal.x + normal.y* normal.y + normal.z* normal.z);

	//float kyori2;
	//kyori2 = 
	//	abs(normal.x * linePos2.x + normal.y * linePos2.y + normal.z * linePos2.z + d) /
	//	sqrt(normal.x* normal.x + normal.y* normal.y + normal.z* normal.z);

	Vector3 vec1 = linePos1 - pointPos;
	Vector3 vec2 = linePos2 - pointPos;

	float kyori1 = 0;
	if (normal.x != 0)kyori1 += abs(Vector3Dot(normal, vec1)) / abs(normal.x);
	if (normal.y != 0)kyori1 += abs(Vector3Dot(normal, vec1)) / abs(normal.y);
	if (normal.z != 0)kyori1 += abs(Vector3Dot(normal, vec1)) / abs(normal.z);
	float kyori2 = 0;
	if (normal.x != 0)kyori2 += abs(Vector3Dot(normal, vec2)) / abs(normal.x);
	if (normal.y != 0)	kyori2 += abs(Vector3Dot(normal, vec2)) / abs(normal.y);
	if (normal.z != 0)	kyori2 += abs(Vector3Dot(normal, vec2)) / abs(normal.z);

	/*float kyori1 = abs(dot(normal, linePos1)) / abs(normal.x);
	float kyori2 = abs(dot(normal, linePos2)) / abs(normal.x);*/

	//内分比
	float a;
	a = kyori1 / (kyori1 + kyori2);


	//ここおかしい

	//線とポリゴンが当たっている場所を調べる
	Vector3 crossVector;//ポリゴンの角から当たってる場所の座標へのベクトル
	crossVector.x = (1 - a) * v1.x + a * v2.x;
	crossVector.y = (1 - a) * v1.y + a * v2.y;
	crossVector.z = (1 - a) * v1.z + a * v2.z;
	Vector3 crossPos = vertexPoint[0] + crossVector;
	//crossPos.y *= - 1;



	//三角形1個目の判定
	Vector3 normal1;
	normal1 = CalcNormal(vertexPoint[0], vertexPoint[1], crossPos);
	Vector3 normal2;
	normal2 = CalcNormal(vertexPoint[1], vertexPoint[2], crossPos);
	Vector3 normal3;
	normal3 = CalcNormal(vertexPoint[2], vertexPoint[0], crossPos);

	//板ポリと法線が同じか調べる
	bool equal1 = false;//板ポリと法線が同じかどうか

	//ほぼ同じだったらtrue
	if (Difference(normal.x, normal1.x, 0.0001f) &&
		Difference(normal.y, normal1.y, 0.0001f) &&
		Difference(normal.z, normal1.z, 0.0001f) &&
		Difference(normal.x, normal2.x, 0.0001f) &&
		Difference(normal.y, normal2.y, 0.0001f) &&
		Difference(normal.z, normal2.z, 0.0001f) &&
		Difference(normal.x, normal3.x, 0.0001f) &&
		Difference(normal.y, normal3.y, 0.0001f) &&
		Difference(normal.z, normal3.z, 0.0001f))
	{
		equal1 = true;
	}


	//三角形2個目の判定
	normal1 = CalcNormal(vertexPoint[2], vertexPoint[1], crossPos);
	normal2 = CalcNormal(vertexPoint[1], vertexPoint[3], crossPos);
	normal3 = CalcNormal(vertexPoint[3], vertexPoint[2], crossPos);

	//板ポリと法線が同じか調べる
	bool equal2 = false;//板ポリと法線が同じかどうか

	//ほぼ同じ(誤差0.0001以内)だったらtrue
	if (Difference(normal.x, normal1.x, 0.0001f) &&
		Difference(normal.y, normal1.y, 0.0001f) &&
		Difference(normal.z, normal1.z, 0.0001f) &&
		Difference(normal.x, normal2.x, 0.0001f) &&
		Difference(normal.y, normal2.y, 0.0001f) &&
		Difference(normal.z, normal2.z, 0.0001f) &&
		Difference(normal.x, normal3.x, 0.0001f) &&
		Difference(normal.y, normal3.y, 0.0001f) &&
		Difference(normal.z, normal3.z, 0.0001f))
	{
		equal2 = true;
	}

	//どちらかが同じ(板ポリの中)だったらifの中に
	if (equal1 || equal2)
	{
		if (crossPosition) *crossPosition = crossPos;
		return true;
	}

	//衝突位置と中心がほぼ同じだったらヒット
	if (Difference(crossPos.x, pointPos.x, 0.01f)
		&& Difference(crossPos.y, pointPos.y, 0.01f) 
		&& Difference(crossPos.z, pointPos.z, 0.01f)
		|| equal1
		|| equal2)
	{
		if (crossPosition) *crossPosition = crossPos;
		return true;
	}


	return false;

}


bool LibMath::RayAndPlaneCollision
(
	const Vector3& layStartPos,
	const Vector3& layDirection,
	const Vector3& normal,
	const float planeDistance,
	float* distance,
	Vector3* crossPos
)
{
	const float epsilon = 1.0e-5f;

	float d1 = Vector3Dot(normal, layDirection);

	if (d1 > -epsilon) return false;

	float d2 = Vector3Dot(normal, layStartPos);

	float dist = d2 - planeDistance;

	float t = dist / -d1;

	if (t < 0)return false;

	if (distance)*distance = t;
	if (crossPos)*crossPos = layStartPos + layDirection * t;

	return true;
}

bool LibMath::RayAndTryangleCollision
(
	const Vector3& layStartPos,
	const Vector3& layDirection,
	const Vector3& triPos1,
	const Vector3& triPos2,
	const Vector3& triPos3,
	const Vector3& normal,
	float* distance,
	Vector3* crossPos
)
{
	Vector3 planeNormal;
	float planeDistance;
	Vector3 interPlane;

	planeNormal = normal;
	planeDistance = Vector3Dot(normal, triPos1);

	if (!RayAndPlaneCollision(layStartPos, layDirection, planeNormal, planeDistance, distance, &interPlane))return false;



	//交差判定
	const float epsilon = 1.0e-5f;
	Vector3 m;

	Vector3 pt_p0 = triPos1 - interPlane;
	Vector3 p0_p1 = triPos2 - triPos1;
	m = Vector3Cross(pt_p0, p0_p1);
	if (Vector3Dot(m, normal) < -epsilon)return false;


	Vector3 pt_p1 = triPos2 - interPlane;
	Vector3 p1_p2 = triPos3 - triPos2;
	m = Vector3Cross(pt_p1, p1_p2);
	if (Vector3Dot(m, normal) < -epsilon)return false;


	Vector3 pt_p2 = triPos3 - interPlane;
	Vector3 p2_p0 = triPos1 - triPos3;
	m = Vector3Cross(pt_p2, p2_p0);
	if (Vector3Dot(m, normal) < -epsilon)return false;

	if (crossPos)*crossPos = interPlane;
	return true;

}

bool LibMath::RayAndSphereCollision
(
	const Vector3& layStartPos,
	const Vector3& layDirection,
	const Vector3& spherePos,
	const float r,
	float* distance,
	Vector3* crossPos
)
{
	Vector3 m = layStartPos - spherePos;
	float b = Vector3Dot(m, layDirection);
	float c = Vector3Dot(m, m) - r * r;
	if (c > 0.0f && b > 0.0f)return false;

	float discr = b * b - c;
	if (discr < 0.0f)return false;


	float t = -b - sqrt(discr);
	if (t < 0)t = 0.0f;
	if (distance)*distance = t;
	if (crossPos)*crossPos = layStartPos + layDirection * t;

	return true;
}

#pragma endregion

#pragma endregion

