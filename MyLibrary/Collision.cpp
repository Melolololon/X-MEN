#include "Collision.h"
#include"LibMath.h"
#include"Values.h"

using namespace MelLib;

Vector3 MelLib::Collision::CalcHitBoxSurfaceNormal(BoxHitDirection dir)
{
	MelLib::Vector3 normal;
	switch (dir)
	{
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT:
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_UP:
		normal = MelLib::Vector3(0, 1, 0);
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_DOWN:
		normal = MelLib::Vector3(0, -1, 0);
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_LEFT:
		normal = MelLib::Vector3(-1, 0, 0);
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_RIGHT:
		normal = MelLib::Vector3(1, 0, 0);
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_FRONT:
		normal = MelLib::Vector3(0, 0, -1);
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_BACK:
		normal = MelLib::Vector3(0, 0, 1);
		break;
	default:
		break;
	}
	return normal;
}

Vector3 MelLib::Collision::CalcSphereArea(const Vector3& spherePos, const Value3<Vector3>& triPos)
{
	Vector3 nearPos;

	Vector3 p1ToSphere = spherePos - triPos.v1;
	Vector3 p2ToSphere = spherePos - triPos.v2;
	Vector3 p3ToSphere = spherePos - triPos.v3;

	// 1
	Vector3 p1ToP2 = triPos.v2 - triPos.v1;
	Vector3 p1ToP3 = triPos.v3 - triPos.v1;

	// 1
	float p1P2DotP1Sphere = Vector3::Dot(p1ToP2, p1ToSphere);
	// 2
	float p1P3DotP1Sphere = Vector3::Dot(p1ToP3, p1ToSphere);
	// 3
	float p1P2DotP2Sphere = Vector3::Dot(p1ToP2, p2ToSphere);
	// 4
	float p1P3DotP2Sphere = Vector3::Dot(p1ToP3, p2ToSphere);
	// 5
	float p1P2DotP3Sphere = Vector3::Dot(p1ToP2, p3ToSphere);
	// 6
	float p1P3DotP3Sphere = Vector3::Dot(p1ToP3, p3ToSphere);


	if (p1P2DotP1Sphere < 0 && p1P3DotP1Sphere < 0)
	{
		nearPos = triPos.v1;
		return nearPos;
	}

	// 2
	Vector3 p2ToP1 = triPos.v1 - triPos.v2;
	Vector3 p2ToP3 = triPos.v3 - triPos.v2;
	if (p1P2DotP2Sphere >= 0 && p1P3DotP2Sphere < p1P2DotP2Sphere)
	{
		nearPos = triPos.v2;
		return nearPos;
	}

	// 4
	Vector3 p3ToP1 = triPos.v1 - triPos.v3;
	Vector3 p3ToP2 = triPos.v2 - triPos.v3;
	if (p1P3DotP3Sphere >= 0 && p1P2DotP3Sphere < p1P3DotP3Sphere)
	{
		nearPos = triPos.v3;
		return nearPos;
	}

	// 3
	float check3 =
		p1P2DotP1Sphere
		* p1P3DotP2Sphere
		- p1P2DotP2Sphere
		* p1P3DotP1Sphere;

	if (p1P2DotP1Sphere >= 0 && p1P2DotP2Sphere < 0)
	{
		if (check3 <= 0.0f)
		{
			nearPos =
				triPos.v1
				+ p1P2DotP1Sphere
				/ (p1P2DotP1Sphere
				- p1P2DotP2Sphere)
				* p1ToP2;

			return nearPos;
		}
	}

	// 5
	float check5 =
		p1P2DotP3Sphere
		* p1P3DotP1Sphere
		- p1P2DotP1Sphere
		* p1P3DotP3Sphere;
	if (p1P3DotP1Sphere >= 0 && p1P3DotP3Sphere < 0)
	{
		if (check5 <= 0.0f)
		{
			nearPos =
				triPos.v1
				+ p1P3DotP1Sphere
				/ (p1P3DotP1Sphere
					- p1P3DotP3Sphere)
				* p1ToP3;
			return nearPos;
		}
	}

	// 6
	float check6 =
		p1P2DotP2Sphere
		* p1P3DotP3Sphere
		- p1P2DotP3Sphere
		* p1P3DotP2Sphere;
	if (p1P3DotP2Sphere - p1P2DotP2Sphere >= 0 && p1P2DotP3Sphere - p1P3DotP3Sphere >= 0)
	{
		if (check6 <= 0.0f)
		{
			float w = (p1P3DotP2Sphere - p1P2DotP2Sphere) / ((p1P3DotP2Sphere - p1P2DotP2Sphere) + (p1P2DotP3Sphere - p1P3DotP3Sphere));

			nearPos =
				triPos.v2
				+ w
				*( triPos.v3 - triPos.v2);
			return nearPos;
		}
	}

	// 7
	float denom = 1.0f / (check3 + check5 + check6);
	float v = check5 * denom;
	float w = check3 * denom;
	nearPos = triPos.v1 + p1ToP2 * v + p1ToP3 * w;
	return nearPos;
}


#pragma region 2D





bool Collision::RectAndRect(const RectData& rect1, const RectData& rect2)
{
	Vector2 pos1 = rect1.GetPosition();
	Vector2 pos2 = rect2.GetPosition();
	Vector2 size1 = rect1.GetSize();
	Vector2 size2 = rect2.GetSize();


	return size2.x + pos2.x > pos1.x &&
		pos2.x < size1.x + pos1.x &&
		pos2.y < size1.y + pos1.y &&
		size2.y + pos2.y > pos1.y;
}

bool Collision::CircleAndCircle(const CircleData& circle1, const CircleData& circle2)
{
	Vector2 pos1 = circle1.GetPosition();
	Vector2 pos2 = circle2.GetPosition();
	float r1 = circle1.GetRadius();
	float r2 = circle2.GetRadius();

	return (pos1.x - pos2.x) *
		(pos1.x - pos2.x) +
		(pos1.y - pos2.y) *
		(pos1.y - pos2.y) <=
		(r1 + r2) * (r1 + r2);
}

bool MelLib::Collision::PointAndCircle(const Vector2& pointPos, const CircleData& circle)
{
	Vector2 circlePos = circle.GetPosition();
	float circleR = circle.GetRadius();

	//半径以内だったらtrue
	return LibMath::CalcDistance2D(pointPos, circlePos) <= circleR;
}

bool MelLib::Collision::PointAndCircularSector(const Vector2& pointPos, const CircularSectorData& circularSector)
{
	//点と円の判定
	if (!PointAndCircle(pointPos, circularSector.GetCircleData()))return false;

	Vector2 circlePos = circularSector.GetCircleData().GetPosition();
	float centerDirAngle = LibMath::Vector2ToAngle(circularSector.GetDirection(), false);
	float pointAngle = LibMath::Vector2ToAngle(LibMath::OtherVector2(circlePos, pointPos), false);
	float centerAngle = circularSector.GetAngle();

	//centerAngle以内だったらtrue
	return LibMath::AngleDifference(centerDirAngle, pointAngle, centerAngle / 2);
}

bool Collision::RectAndCircle(const RectData& rect, const CircleData& circle)
{
	return false;
}

bool Collision::CircleAndSegment2D
(
	const CircleData& circle,
	CircleCalcResult* circleCalcResult,
	const Segment2DData& lineSegment,
	Segment2DCalcResult* lineSegmentCalcResult
)
{
	SegmentHitPlace hitPlace = SegmentHitPlace::LS_HIT_POSITION_NOT_HIT;
	if (lineSegmentCalcResult)
	{
		lineSegmentCalcResult->hitPlace = hitPlace;
	}

	Value2<Vector2>segmentPos = lineSegment.GetPosition();
	Vector2 circlePos = circle.GetPosition();
	float circleR = circle.GetRadius();

	//線の端から端へのベクトル
	Vector2 lineVector = segmentPos.v2 - segmentPos.v1;
	//線の座標1から円の中心
	Vector2 linePos0ToSphere = circlePos - segmentPos.v1;
	//円と線の最短距離を求める
	float distance = abs(Vector2::Cross(lineVector, linePos0ToSphere) / Vector2::Length(lineVector));


	//円と線分の最近点を求める
	//これ関数化する

	//linePos1から最近点の距離を求める
	float linePos0ToNearPosDis = Vector2::Dot(Vector2::Normalize(lineVector), linePos0ToSphere);
	if (lineSegmentCalcResult)
	{
		lineSegmentCalcResult->nearPos = segmentPos.v1 + (Vector2::Normalize(lineVector) * linePos0ToNearPosDis);
	}

	//距離のほうが大きかったら計算終了
	if (distance > circleR)return false;

	//線の座標2から円の中心
	Vector2 linePos2ToSphere = circlePos - segmentPos.v2;
	//正の数かどうかを判別するフラグ
	bool linePos1ToSpherePositive = false;
	bool linePos2ToSpherePositive = false;

	//内積を求める
	float linePos1ToSphereDot = Vector2::Dot(lineVector, linePos0ToSphere);
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
		float linePos1ToSphereLength = Vector2::Length(linePos0ToSphere);
		float linePos2ToSphereLength = Vector2::Length(linePos2ToSphere);

		//半径より大きかったら当たってない
		if (linePos1ToSphereLength > circleR
			&& linePos2ToSphereLength > circleR)
		{
			return false;
		}

		hitPlace = SegmentHitPlace::LS_HIT_POSITION_LE_START_END;
	}
	else
	{
		hitPlace = SegmentHitPlace::LS_HIT_POSITION_LE_LINE;
	}

	if (lineSegmentCalcResult)
	{
		lineSegmentCalcResult->hitPlace = hitPlace;
	}

	return true;
}


#pragma endregion

#pragma region 3D

bool Collision::SphereAndSphere(const SphereData& sphere1, const SphereData& sphere2)
{
	Vector3 sphere1Pos = sphere1.GetPosition();
	float sphere1R = sphere1.GetRadius();
	Vector3 sphere2Pos = sphere2.GetPosition();
	float sphere2R = sphere2.GetRadius();

	return (sphere2Pos.x - sphere1Pos.x) *
		(sphere2Pos.x - sphere1Pos.x) +
		(sphere2Pos.y - sphere1Pos.y) *
		(sphere2Pos.y - sphere1Pos.y) +
		(sphere2Pos.z - sphere1Pos.z) *
		(sphere2Pos.z - sphere1Pos.z) <=
		(sphere1R + sphere2R) * (sphere1R + sphere2R);
}

bool Collision::BoxAndBox
(
	const BoxData& box1,
	BoxCalcResult* boxCalcResult1,
	const BoxData& box2,
	BoxCalcResult* boxCalcResult2
)
{
	Vector3 box1Pos = box1.GetPosition();
	Vector3 box1Size = box1.GetSize();
	Vector3 box2Pos = box2.GetPosition();
	Vector3 box2Size = box2.GetSize();

	Vector3 minPos1 = box1Pos - box1Size / 2;
	Vector3 maxPos1 = box1Pos + box1Size / 2;
	Vector3 minPos2 = box2Pos - box2Size / 2;
	Vector3 maxPos2 = box2Pos + box2Size / 2;
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
			{
				isHit = true;
			}

		}
	}

	if (boxCalcResult1 || boxCalcResult2)
	{
		BoxHitDirection direction1 = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
		BoxHitDirection direction2 = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;

		if (!isHit)
		{
			if (boxCalcResult1) boxCalcResult1->boxHitDistance = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			if (boxCalcResult2) boxCalcResult2->boxHitDistance = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			return false;
		}

		//1 Xが多い
		//2 Yが多い
		//3 Zが多い
		char top = 0;
		//対象へのベクトル
		Vector3 targetToVector = box2Pos - box1Pos;

		if (abs(targetToVector.x) > abs(targetToVector.y) &&
			abs(targetToVector.x) > box2Size.x / 2)
		{
			top = 1;
			if (abs(targetToVector.z) > abs(targetToVector.x) &&
				abs(targetToVector.z) > box2Size.z / 2)
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(targetToVector.z) > abs(targetToVector.y) &&
				abs(targetToVector.z) > box2Size.z / 2)
				top = 3;
		}


		if (top == 1)
		{
			if (targetToVector.x >= 0)
			{

				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			}
			else
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
			}
		}
		else if (top == 2)
		{
			if (targetToVector.y >= 0)
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_UP;
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			}
			else
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_UP;
			}
		}
		else if (top == 3)
		{
			if (targetToVector.z >= 0)
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			}
			else
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
			}
		}

		if (boxCalcResult1) boxCalcResult1->boxHitDistance = direction1;
		if (boxCalcResult2) boxCalcResult2->boxHitDistance = direction2;
	}

	return isHit;
}

bool Collision::Segment3DAndSegment3D
(
	const Segment3DData& lineSegment1,
	Segment3DCalcResult* lineSegment1CalcResult,
	const Segment3DData& lineSegment2,
	Segment3DCalcResult* lineSegment2CalcResult
)
{
	Value2<Vector3>segmentPos1 = lineSegment1.GetRotatePosition();

	Value2<Vector3>segmentPos2 = lineSegment2.GetRotatePosition();


	Vector3 d1 = segmentPos1.v2 - segmentPos1.v1;
	Vector3 d2 = segmentPos2.v2 - segmentPos2.v1;
	//カプセルの処理の一部を使えば求められる?
	Vector3 r = segmentPos1.v1 - segmentPos2.v1;
	float a = Vector3::Dot(d1, d1);
	float b = Vector3::Dot(d1, d2);
	float c = Vector3::Dot(d1, r);
	float e = Vector3::Dot(d2, d2);
	float f = Vector3::Dot(d2, r);
	float d = a * e - b * b;

	float s = 0.0f;
	if (d != 0.0f)
	{
		s = (b * f - c * e) / d;

		if (s < 0.0f)s = 0.0f;
		if (s > 1.0f)s = 1.0f;
	}

	Vector3 p1 = segmentPos1.v1;
	Vector3 p2 = segmentPos2.v1;
	float t = Vector3::Dot(((p1 + d1 * s) - p2), d2) / Vector3::Dot(d2, d2);

	s = (t * b - c) / a;
	if (t < 0.0f)
	{
		t = 0.0f;
		s = -c / a;
	}
	else if (t > 1.0f)
	{
		t = 1.0f;
		s = (b - c) / a;
	}
	if (s < 0.0f)s = 0.0f;
	if (s > 1.0f)s = 1.0f;

	//相手に一番近い場所
	Vector3 c1 = segmentPos1.v1 + s * d1;
	Vector3 c2 = segmentPos2.v1 + t * d2;

	//近い場所の距離が0だったら当たっている(重なっている、交差している)
	bool isHit = LibMath::CalcDistance3D(c1, c2) == 0.0f;

	if (lineSegment1CalcResult)lineSegment1CalcResult->lineSegment3DHitPos = c1;
	if (lineSegment2CalcResult)lineSegment2CalcResult->lineSegment3DHitPos = c2;
	return isHit;

}

bool Collision::CapsuleAndCapsule(const CapsuleData& capsule1, const CapsuleData& capsule2)
{
	//回転適応.
	Value2<Vector3>capsule1LineSegmentPos = capsule1.GetSegment3DData().GetRotatePosition();

	Value2<Vector3>capsule2LineSegmentPos = capsule2.GetSegment3DData().GetRotatePosition();


	Vector3 d1 = capsule1LineSegmentPos.v2 - capsule1LineSegmentPos.v1;
	Vector3 d2 = capsule2LineSegmentPos.v2 - capsule2LineSegmentPos.v1;

	Vector3 r = capsule1LineSegmentPos.v1 - capsule2LineSegmentPos.v1;
	float a = Vector3::Dot(d1, d1);
	float b = Vector3::Dot(d1, d2);
	float c = Vector3::Dot(d1, r);
	float e = Vector3::Dot(d2, d2);
	float f = Vector3::Dot(d2, r);
	float d = a * e - b * b;

	float s = 0.0f;
	if (d != 0.0f)
	{
		s = (b * f - c * e) / d;

		if (s < 0.0f)s = 0.0f;
		if (s > 1.0f)s = 1.0f;
	}

	Vector3 p1 = capsule1LineSegmentPos.v1;
	Vector3 p2 = capsule2LineSegmentPos.v1;
	float t = Vector3::Dot(((p1 + d1 * s) - p2), d2) / Vector3::Dot(d2, d2);

	s = (t * b - c) / a;
	if (t < 0.0f)
	{
		t = 0.0f;
		s = -c / a;
	}
	else if (t > 1.0f)
	{
		t = 1.0f;
		s = (b - c) / a;
	}
	if (s < 0.0f)s = 0.0f;
	if (s > 1.0f)s = 1.0f;

	//相手カプセルに一番近い場所
	Vector3 c1 = p1 + s * d1;
	Vector3 c2 = p2 + t * d2;

	return LibMath::CalcDistance3D(c1, c2) < capsule1.GetRadius() + capsule2.GetRadius();
}

bool Collision::SphereAndBox
(
	const SphereData& sphere,
	SphereCalcResult* sphereCalcResult,
	const BoxData& box,
	BoxCalcResult* boxCalcResult
)
{
	float dir2 = 0.0f;

	Vector3 minPos = box.GetPosition() - box.GetSize() / 2;
	Vector3 maxPos = box.GetPosition() + box.GetSize() / 2;

	//x
	if (sphere.GetPosition().x < minPos.x)
		dir2 += (minPos.x - sphere.GetPosition().x) * (minPos.x - sphere.GetPosition().x);
	if (sphere.GetPosition().x > maxPos.x)
		dir2 += (sphere.GetPosition().x - maxPos.x) * (sphere.GetPosition().x - maxPos.x);

	//y
	if (sphere.GetPosition().y < minPos.y)
		dir2 += (minPos.y - sphere.GetPosition().y) * (minPos.y - sphere.GetPosition().y);
	if (sphere.GetPosition().y > maxPos.y)
		dir2 += (sphere.GetPosition().y - maxPos.y) * (sphere.GetPosition().y - maxPos.y);

	//z
	if (sphere.GetPosition().z < minPos.z)
		dir2 += (minPos.z - sphere.GetPosition().z) * (minPos.z - sphere.GetPosition().z);
	if (sphere.GetPosition().z > maxPos.z)
		dir2 += (sphere.GetPosition().z - maxPos.z) * (sphere.GetPosition().z - maxPos.z);

	bool isHit = dir2 < sphere.GetRadius()* sphere.GetRadius();

	//どこに当たったかを返す
	if (sphereCalcResult || boxCalcResult)
	{
		//箱のどの部分にぶつかったかという情報
		BoxHitDirection hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
		if (!isHit)
		{
			sphereCalcResult->SetBoxHitDirection(hitDirection);
			boxCalcResult->boxHitDistance = hitDirection;
			return false;
		}

		//1 Xが多い
		//2 Yが多い
		//3 Zが多い
		char top = 0;
		//ボックスへのベクトル
		Vector3 sphereToVector = sphere.GetPosition();

		if (abs(sphereToVector.x) >= abs(sphereToVector.y) &&
			abs(sphereToVector.x) >= box.GetSize().x / 2)
		{
			top = 1;
			if (abs(sphereToVector.z) >= abs(sphereToVector.x) &&
				abs(sphereToVector.z) >= box.GetSize().z / 2)
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(sphereToVector.z) >= abs(sphereToVector.y) &&
				abs(sphereToVector.z) >= box.GetSize().z / 2)
				top = 3;
		}

		if (top == 1)
		{
			if (sphereToVector.x >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			}
		}
		else if (top == 2)
		{
			if (sphereToVector.y >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_UP;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			}
		}
		else if (top == 3)
		{
			if (sphereToVector.z >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			}
		}



		if (sphereCalcResult)
		{
			sphereCalcResult->SetBoxHitDirection(hitDirection);
			sphereCalcResult->SetBoxHitSurfaceNormal(CalcHitBoxSurfaceNormal(hitDirection));
		}
		if (boxCalcResult)boxCalcResult->boxHitDistance = hitDirection;
	}

	return isHit;
}

bool MelLib::Collision::SphereAndOBB(const SphereData& sphere, SphereCalcResult* sphereCalcResult, const OBBData& obb)
{

	// L
	Vector3 sizeHalf = obb.GetBoxData().GetSize() / 2;

	// 回転した軸3本(XYZ)方向にはみ出ているか確認する
	// XYZの軸にOBBの回転を適応させる
	// v1,v2,v3
	Vector3 angle = obb.GetAngle();
	Vector3 axisX = LibMath::RotateZXYVector3(Vector3(1, 0, 0), angle);
	Vector3 axisY = LibMath::RotateZXYVector3(Vector3(0, 1, 0), angle);
	Vector3 axisZ = LibMath::RotateZXYVector3(Vector3(0, 0, 1), angle);

	// GP
	// 箱の中心から球の中心へのベクトル
	Vector3 boxToSphere = sphere.GetPosition() - obb.GetBoxData().GetPosition();

	float s1 = Vector3::Dot(boxToSphere, axisX) / sizeHalf.x;
	s1 = abs(s1);
	if (s1 < 1)s1 = 1;

	float s2 = Vector3::Dot(boxToSphere, axisY) / sizeHalf.y;
	s2 = abs(s2);
	if (s2 < 1)s2 = 1;

	float s3 = Vector3::Dot(boxToSphere, axisZ) / sizeHalf.z;
	s3 = abs(s3);
	if (s3 < 1)s3 = 1;

	float dis = 0.0f;
	dis = (
		(1 - s1) * sizeHalf.x * axisX
		+ (1 - s2)* sizeHalf.y * axisY
		+ (1 - s3) * sizeHalf.z * axisZ
		).Length();

	bool result = dis <= sphere.GetRadius();
	

	
	if (!sphereCalcResult || !result)return result;
	
	// 以下法線計算
	
	// 法線取得どうする
	// 四角形の角度を0,0,0の時に戻し、球の位置も戻した分回転。(箱はGetBoxDataで取得すればいいので、回転させる必要はない)
	// Boxと同じように求める
	// 法線を回転
	
	BoxHitDirection hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
	
	BoxData box = obb.GetBoxData();
	//1 Xが多い
	//2 Yが多い
	//3 Zが多い
	char top = 0;

	// 角度のマイナス分回転
	// OBBを基準に回転させないといけない
	Vector3 rotSphere = LibMath::RotateZXYVector3(boxToSphere, -angle);
	rotSphere += box.GetPosition();

	//ボックスへのベクトル
	Vector3 sphereToVector = rotSphere;

	if (abs(sphereToVector.x) >= abs(sphereToVector.y) &&
		abs(sphereToVector.x) >= box.GetSize().x / 2)
	{
		top = 1;
		if (abs(sphereToVector.z) >= abs(sphereToVector.x) &&
			abs(sphereToVector.z) >= box.GetSize().z / 2)
			top = 3;
	}
	else
	{
		top = 2;
		if (abs(sphereToVector.z) >= abs(sphereToVector.y) &&
			abs(sphereToVector.z) >= box.GetSize().z / 2)
			top = 3;
	}

	if (top == 1)
	{
		if (sphereToVector.x >= 0)
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
		}
		else
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
		}
	}
	else if (top == 2)
	{
		if (sphereToVector.y >= 0)
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_UP;
		}
		else
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
		}
	}
	else if (top == 3)
	{
		if (sphereToVector.z >= 0)
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
		}
		else
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
		}
	}

	
	// 法線を求めてセット
	Vector3 normal = CalcHitBoxSurfaceNormal(hitDirection);
	sphereCalcResult->SetOBBHitSurfaceNormal(LibMath::RotateZXYVector3(normal, angle));
	
	return result;
}

bool MelLib::Collision::SphereAndRay(const SphereData& sphere, SphereCalcResult* sphereCalcResult, const RayData& ray, RayCalcResult* rayResult)
{
	Vector3 spherePos = sphere.GetPosition();
	float sphereR = sphere.GetRadius();

	Vector3 rayPos = ray.GetPosition();
	Vector3 rayDir = ray.GetDirection();

	Vector3 sphereToRay = rayPos - spherePos;
	
	float b = Vector3::Dot(sphereToRay, rayDir);
	float c = Vector3::Dot(sphereToRay, sphereToRay) - (sphereR * sphereR);

	float d = (b * b) - c;

	if (d < 0)return false;

	float t = -b - sqrt((b * b) - c);
	if (t < 0)return false;
	
	if (d > 0)
	{
		if (rayResult)
		{
			Vector3 hitPosition = rayPos + t * rayDir;
			rayResult->hitPosition = hitPosition;

			rayResult->hitObjectNormal = (hitPosition) / Vector3::Normalize(hitPosition);
		}
	}
	return true;
}

bool Collision::SphereAndCapsule(const SphereData& sphere, const CapsuleData& capsule)
{
	Value2<Vector3>capsuleLineSegmentPos = capsule.GetSegment3DData().GetRotatePosition();
	Vector3 spherePos = sphere.GetPosition();

	Vector3 capsulePos0ToSphere = spherePos - capsuleLineSegmentPos.v1;
		//LibMath::OtherVector3(capsuleLineSegmentPos.v1, spherePos);

	//資料のn
	Vector3 capsuleLineSegmentVector = capsuleLineSegmentPos.v2 - capsuleLineSegmentPos.v1;
		//LibMath::OtherVector3(capsuleLineSegmentPos.v1, capsuleLineSegmentPos.v2);

	float t = Vector3::Dot(capsulePos0ToSphere, capsuleLineSegmentVector.Normalize());

	//球から線分に垂直に下した線上の座標 資料のPs→Pn
	Vector3 startToLineUpVector = capsuleLineSegmentVector.Normalize() * t;
	Vector3 onTheLinePos = capsuleLineSegmentPos.v1 + startToLineUpVector;

	float lenghtRate = t / capsuleLineSegmentVector.Length();
	float sphereAndCupsuleDis = 0.0f;
	if (lenghtRate < 0.0f)
	{
		sphereAndCupsuleDis = LibMath::CalcDistance3D(spherePos, capsuleLineSegmentPos.v1);
	}
	else if (lenghtRate > 1.0f)
	{
		sphereAndCupsuleDis = LibMath::CalcDistance3D(spherePos, capsuleLineSegmentPos.v2);
	}
	else
	{
		sphereAndCupsuleDis = LibMath::CalcDistance3D(spherePos, onTheLinePos);
	}

	return sphereAndCupsuleDis < sphere.GetRadius();
}

bool MelLib::Collision::SphereAndTriangle(const SphereData& sphere, const TriangleData& triangle)
{
	Vector3 nearPos;
	nearPos = CalcSphereArea(sphere.GetPosition(), triangle.GetTranslationPosition());

	float dis = LibMath::CalcDistance3D(nearPos, sphere.GetPosition());
	dis *= dis;
	float radius = sphere.GetRadius() * sphere.GetRadius();
	return dis <= radius;
}

bool MelLib::Collision::PlaneAndSegment3D(const PlaneData& plane, const Segment3DData& segment, Segment3DCalcResult* segmentResult)
{
	Vector3 planeToSegV1 = segment.GetPosition().v1 - plane.GetPosition();
	Vector3 planeToSegV2 = segment.GetPosition().v2 - plane.GetPosition();

	bool result = Vector3::Dot(planeToSegV1, plane.GetNormal()) * Vector3::Dot(planeToSegV2, plane.GetNormal()) <= 0;

	if (segmentResult)
	{
		MelLib::Vector3 vec1 = segment.GetPosition().v1 - plane.GetPosition();
		MelLib::Vector3 vec2 = segment.GetPosition().v2 - plane.GetPosition();

		//内分比を求め、衝突点を求める
		float planeToSegV1Dis = abs(Vector3::Dot(plane.GetNormal(), vec1)) / plane.GetNormal().Length();
		float planeToSegV2Dis = abs(Vector3::Dot(plane.GetNormal(), vec2)) / plane.GetNormal().Length();
		float naibunhi = planeToSegV1Dis / (planeToSegV1Dis + planeToSegV2Dis);

		segmentResult->planeHitPos = (1 - naibunhi) * segment.GetPosition().v1 + naibunhi * segment.GetPosition().v2;
	}
	return result;
}

bool Collision::BoardAndSegment3D
(
	const BoardData& board,
	BoardCalcResult* boardCalcResult,
	const Segment3DData& lineSegment,
	Segment3DCalcResult* lineSegmentCalcResult
)
{

	Value2<Vector3> segmentPos = lineSegment.GetRotatePosition();
	Value4<Vector3>boardVertexPos = board.GetVertexPosition();
	Vector3 leftDownPos = boardVertexPos.v1;
	Vector3 leftUpPos = boardVertexPos.v2;
	Vector3 rightDownPos = boardVertexPos.v3;
	Vector3 rightUpPos = boardVertexPos.v4;

	Vector3 v1;
	Vector3 v2;

	v1 = segmentPos.v1 - board.GetPosition();
	v2 = segmentPos.v2 - board.GetPosition();

	//線が板ポリと並行ではないかを調べる(平行だったらreturn)
	if (Vector3Dot(v1, board.GetNormal()) * Vector3Dot(v2, board.GetNormal()) > 0)
		return false;


	//ここにポリゴンの範囲内かどうかの処理を書く

	//線の端 - ポリゴンの角
	v1 = segmentPos.v1 - leftDownPos;
	v2 = segmentPos.v2 - leftDownPos;

	Vector3 vec1 = segmentPos.v1 - board.GetPosition();
	Vector3 vec2 = segmentPos.v2 - board.GetPosition();

	//線分の両端と板ポリの距離を求める
	//0で割るのを防止するためのif
	float kyori1 = 0;
	/*if (board.GetNormal().x != 0)kyori1 += abs(Vector3Dot(board.GetNormal(), vec1)) / abs(board.GetNormal().x);
	if (board.GetNormal().y != 0)kyori1 += abs(Vector3Dot(board.GetNormal(), vec1)) / abs(board.GetNormal().y);
	if (board.GetNormal().z != 0)kyori1 += abs(Vector3Dot(board.GetNormal(), vec1)) / abs(board.GetNormal().z);*/
	kyori1 = abs(Vector3Dot(board.GetNormal(), vec1)) / board.GetNormal().Length();
	float kyori2 = 0;
	/*if (board.GetNormal().x != 0)kyori2 += abs(Vector3Dot(board.GetNormal(), vec2)) / abs(board.GetNormal().x);
	if (board.GetNormal().y != 0)kyori2 += abs(Vector3Dot(board.GetNormal(), vec2)) / abs(board.GetNormal().y);
	if (board.GetNormal().z != 0)kyori2 += abs(Vector3Dot(board.GetNormal(), vec2)) / abs(board.GetNormal().z);*/
	kyori2 = abs(Vector3Dot(board.GetNormal(), vec2)) / board.GetNormal().Length();

	//内分比
	float a = kyori1 / (kyori1 + kyori2);



	//線とポリゴンが当たっている場所を調べる
	Vector3 crossVector;//ポリゴンの角から当たってる場所の座標へのベクトル
	crossVector.x = (1 - a) * v1.x + a * v2.x;
	crossVector.y = (1 - a) * v1.y + a * v2.y;
	crossVector.z = (1 - a) * v1.z + a * v2.z;
	Vector3 crossPos = leftDownPos + crossVector;




	//三角形1個目の判定
	Vector3 normal1;
	normal1 = LibMath::CalcNormal(leftDownPos, leftUpPos, crossPos);
	Vector3 normal2;
	normal2 = LibMath::CalcNormal(leftUpPos, rightDownPos, crossPos);
	Vector3 normal3;
	normal3 = LibMath::CalcNormal(rightDownPos, leftDownPos, crossPos);

	//板ポリと法線が同じか調べる
	bool equal1 = false;//板ポリと法線が同じかどうか

	//ほぼ同じだったらtrue
	if (LibMath::Difference(board.GetNormal().x, normal1.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal1.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal1.z, 0.0001f) &&
		LibMath::Difference(board.GetNormal().x, normal2.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal2.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal2.z, 0.0001f) &&
		LibMath::Difference(board.GetNormal().x, normal3.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal3.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal3.z, 0.0001f))
	{
		equal1 = true;
	}


	//三角形2個目の判定
	normal1 = LibMath::CalcNormal(rightDownPos, leftUpPos, crossPos);
	normal2 = LibMath::CalcNormal(leftUpPos, rightUpPos, crossPos);
	normal3 = LibMath::CalcNormal(rightUpPos, rightDownPos, crossPos);

	//板ポリと法線が同じか調べる
	bool equal2 = false;//板ポリと法線が同じかどうか

	//ほぼ同じ(誤差0.0001以内)だったらtrue
	if (LibMath::Difference(board.GetNormal().x, normal1.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal1.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal1.z, 0.0001f) &&
		LibMath::Difference(board.GetNormal().x, normal2.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal2.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal2.z, 0.0001f) &&
		LibMath::Difference(board.GetNormal().x, normal3.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal3.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal3.z, 0.0001f))
	{
		equal2 = true;
	}

	//どちらかが同じ(板ポリの中)だったらifの中に
	if (equal1 || equal2)
	{
		if (lineSegmentCalcResult) lineSegmentCalcResult->boardHitPos = crossPos;
		if (boardCalcResult)boardCalcResult->lineSegment3DHitPos = crossPos;
		return true;
	}

	//衝突位置と中心がほぼ同じだったらヒット
	if (LibMath::Difference(crossPos.x, board.GetPosition().x, 0.001f)
		&& LibMath::Difference(crossPos.y, board.GetPosition().y, 0.001f)
		&& LibMath::Difference(crossPos.z, board.GetPosition().z, 0.001f)
		|| equal1
		|| equal2)
	{
		if (lineSegmentCalcResult) lineSegmentCalcResult->boardHitPos = crossPos;
		if (boardCalcResult)boardCalcResult->lineSegment3DHitPos = crossPos;
		return true;
	}
	return false;

}

bool MelLib::Collision::BoardAndCapsule(const BoardData& board, BoardCalcResult* boardCalcResult, const CapsuleData& capsule, Segment3DCalcResult* lineSegmentCalcResult)
{
	//線分の判定が成立している、または、距離が半径以内
	//後者が成立してるかを判断すればいい

	//距離をどう求めるか

	//これ実装できれば、壁との判定もとれる

	//両端の辺の方向に板ポリゴンを半径分動かして、交差したら、
	//カプセルと当たってるって言える?
	//中心でもいい?
	//この方法じゃダメだった

	//衝突点は交差してなくても求められる?(平面扱いして計算してるならできるはず)
	//衝突点と四つの辺の距離が半径以内、または、平面との距離が半径以内だったらOK?
	//それだと、範囲外でも平面と交差してたらアウトだからダメ


	//じゃあ、四角形の範囲内だったら、両端と平面との距離を計算
	//範囲外だったら、辺との距離を計算
	//それだと、カプセルと板が平行の時アウト
	//平行だったら範囲内でも平面との距離を計算する?
	//あと、斜めになってて範囲外にいてかつ中心の近くにいて板がクソデカかったらアウト
	//その場合、両端と平面の距離を求めればいい?
	//そもそも、めちゃくちゃ長かったら、両端の距離求めても意味ない?

	//範囲外の時、辺との距離を計算。それでだめだったら両端の距離を求めればいい?(球並みに短くて半径でかいやつ用)
	//範囲外で両端の距離を求める場合、衝突点が中にあるか確認しないといけない?
	//そもそも衝突点が外になる場合は、線分同士のチェックに引っかかるかあたらない
	//クソ長いのが刺さってたら、両端と平面の距離求めても意味ない場合あるから、刺さってるかどうかも判断して、
	//刺さってたら当たってる。刺さってなかったら両端求めればいい。
	//平行の時、単純に距離を求めると、当たってないのにあたってることになる


	//中の場合、
	//1.刺さっている
	//2.両端と平面の距離が半径以内
	//どちらかが成立してればおそらく当たっている

	//外の場合、
	//1.線分と板の辺の距離が半径以内
	// 
	//普通に両端と平面の距離を計算すると、板からめちゃくちゃ離れてても平面に近かったらダメ
	//2.


	//難しく考えすぎ?
	//刺さってるか確認し、



	//https://www.antun.net/tips/algorithm/collision.html
	//線分と三角形の距離を求めればいけそう。
	//(四角形は三角形を二つ合わせたものなので)
	//書かれてるの2Dのやつだった


	Value2<Vector3> segmentPos = capsule.GetSegment3DData().GetRotatePosition();
	Value4<Vector3>boardVertexPos = board.GetVertexPosition();
	Vector3 leftDownPos = boardVertexPos.v1;
	Vector3 leftUpPos = boardVertexPos.v2;
	Vector3 rightDownPos = boardVertexPos.v3;
	Vector3 rightUpPos = boardVertexPos.v4;

	Vector3 v1;
	Vector3 v2;

	v1 = segmentPos.v1 - board.GetPosition();
	v2 = segmentPos.v2 - board.GetPosition();

	//線が板ポリと並行ではないかを調べる(平行だったらreturn)
	if (Vector3Dot(v1, board.GetNormal()) * Vector3Dot(v2, board.GetNormal()) > 0)
	{
		//平行だった場合、両端と平面の距離が半径以内だったら当たってる
		float planeDir = abs(Vector3::Dot(board.GetNormal(), segmentPos.v1 - board.GetPosition())) / board.GetNormal().Length();
		if (planeDir <= capsule.GetRadius())return true;

		planeDir = abs(Vector3::Dot(board.GetNormal(), segmentPos.v2 - board.GetPosition())) / board.GetNormal().Length();
		if (planeDir <= capsule.GetRadius())return true;

		return false;
	}


	//起きたらやること
	//範囲内か外かを調べて処理を行う


}

bool MelLib::Collision::PointAndSphere(const Vector3& pointPos, const SphereData& sphere)
{
	return LibMath::CalcDistance3D(pointPos, sphere.GetPosition()) <= sphere.GetRadius();
}

//
//bool MelLib::Collision::PointAndCircularSector3D(const Vector3& pointPos, const CircularSector3DData& circularSector)
//{
//	//球の中にあるかどうか確認
//	if (!PointAndSphere(pointPos, circularSector.GetSphereData()))return;
//
//	//やり方
//	//1.
//	//縦と横の角度に応じて、ボードを回転する?
//	//どうやって中にあるか確認するの?
//	//表裏の判定を行えば確認できる
//
//	//2.
//	//扇形を2つ持たせる?
//	//
//
//	BoardData nearBoard;
//	BoardData farBoard;
//	BoardData leftBoard;
//	BoardData rightBoard;
//	BoardData upBoard;
//	BoardData downBoard;
//
//}
//
//#pragma endregion
//
//

bool MelLib::Collision::PointAndFrustum(const Vector3& pointPos, const FrustumData& frustum)
{
	//点が、錐台の全ての面の裏側にあったらtrue

	std::vector<BoardData>boards = frustum.GetBoardDatas();

	for (const auto& b : boards)
	{
		//表側にあったらfalse
		if (LibMath::PointBoardFrontBackCheck(pointPos, b) == 1)
		{
			return false;
		}
	}
	return true;
}

bool MelLib::Collision::BoxAndRay(const BoxData& box, const RayData& ray, RayCalcResult* rayResult)
{
	// https://plaza.rakuten.co.jp/ootorii/diary/200705140000/

	// https://el-ement.com/blog/2017/08/16/primitives-ray-intersection/

	// http://marupeke296.com/COL_3D_No18_LineAndAABB.html
	
	// いいサイト
	// https://tavianator.com/2011/ray_box.html


	Vector3 rayPos = ray.GetPosition();
	Vector3 rayDir = ray.GetDirection();

	MelLib::Vector3 minPos, maxPos;
	minPos = box.GetPosition() - box.GetSize() / 2;
	maxPos = box.GetPosition() + box.GetSize() / 2;

	float t = -FLT_MAX;

	// floatで表現できる値の最小値と最大値を取得
	// x軸の判定をする際(一番はじめの軸との判定時に使う)
	// それ以降は一つ前の軸との距離と判定して各軸のスラブ内で交差しているか判定
	float minT = -FLT_MAX;
	float maxT = FLT_MAX;

	for (int i = 0; i < 3; i++) 
	{
		if (LibMath::Difference(rayDir[i], 0.0f, 0.0001f))
		{
			// 明らかに出てたら当たってない判定
			if (rayPos[i] < minPos[i] || rayPos[i] > maxPos[i])return false;
			
			// 上のfalseに引っかからなかったら当たってるため、次へ
			continue;
		}

		// 掛け算のほうが早いため、逆数(invNum)を求めて掛け算で計算する
		// 逆数
		float invNum = 1.0f / rayDir[i];

		// スラブとの距離を算出
		// nearTが近スラブ、farTが遠スラブとの距離
		float nearT = (minPos[i] - rayPos[i]) * invNum;
		float farT = (maxPos[i] - rayPos[i]) * invNum;

		float tMin = min(nearT, farT);
		float tMax = max(nearT, farT);
		nearT = tMin;
		farT = tMax;

		if (nearT > minT) minT = nearT;
		if (farT < maxT) maxT = farT;

		// スラブ交差チェック
		if (minT >= maxT)return false;
	}
	
	// 間違えてここで0以上か判断しちゃってるけど問題なさそう?
	// ベクトルの逆にOBBがあるとき、絶対Tが逆になるからこれで問題なさそう
	if (minT >= 0 && maxT >= 0) 
	{
		if (rayResult)
		{
			Vector3 hitPos = rayPos + minT * rayDir;
			rayResult->hitPosition = hitPos;


			BoxHitDirection hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;

			char top = 0;
			Vector3 hitPosVector = hitPos;

			if (abs(hitPosVector.x) >= abs(hitPosVector.y)/* &&
				abs(hitPosVector.x) >= box.GetSize().x / 2*/)
			{
				top = 1;
				if (abs(hitPosVector.z) >= abs(hitPosVector.x) /*&&
					abs(hitPosVector.z) >= box.GetSize().z / 2*/)
					top = 3;
			}
			else
			{
				top = 2;
				if (abs(hitPosVector.z) >= abs(hitPosVector.y)/* &&
					abs(hitPosVector.z) >= box.GetSize().z / 2*/)
					top = 3;
			}

			if (top == 1)
			{
				if (hitPosVector.x >= 0)
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
				}
				else
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
				}
			}
			else if (top == 2)
			{
				if (hitPosVector.y >= 0)
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_UP;
				}
				else
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
				}
			}
			else if (top == 3)
			{
				if (hitPosVector.z >= 0)
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
				}
				else
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
				}
			}


			rayResult->hitObjectNormal = CalcHitBoxSurfaceNormal(hitDirection);
		}
		return true;
	}
	return false;
}

bool MelLib::Collision::BoxAndSegment3D(const BoxData& box, const Segment3DData& segment)
{
	RayData ray;
	ray.SetPosition(segment.GetPosition().v1);
	ray.SetDirection((segment.GetPosition().v2 - segment.GetPosition().v1).Normalize());
	RayCalcResult result;

	if (!BoxAndRay(box, ray, &result))return false;

	// 衝突点が線分と重なってたら当たってる判定
	if (PointAndSegment3D(result.hitPosition, segment))return true;
	return false;

}

bool MelLib::Collision::PointAndSegment3D(const Vector3& pointPos, const Segment3DData& segment)
{
	Vector3 segmentPosVector = segment.GetRotatePosition().v2 - segment.GetRotatePosition().v1;
	Vector3 segmentV1ToPoint = pointPos - segment.GetRotatePosition().v1;

	float v1 = segmentV1ToPoint.Length();
	float v2 = segmentPosVector.Length();
	return LibMath::Difference(Vector3::Cross(segmentV1ToPoint, segmentPosVector).Length(), 0.0f, 0.0001f)
		&& segmentV1ToPoint.Length() <= segmentPosVector.Length();

}

bool MelLib::Collision::TriangleAndSegment3D(const TriangleData& triangle, TriangleCalcResult* triangleCalcResult, const Segment3DData& lineSegment, Segment3DCalcResult* lineSegmentCalcResult)
{
	Value2<Vector3> segmentPos = lineSegment.GetRotatePosition();

	Value3<Vector3>triangleVertexPos = triangle.GetTranslationPosition();

	// 回転と平行移動を適応させた三角形の中心座標を取得
	Vector3 centerPos = triangle.GetRotTranceFormCenter();

	Vector3 v1;
	Vector3 v2;

	v1 = segmentPos.v1 - centerPos;
	v2 = segmentPos.v2 - centerPos;

	//線が三角形と並行ではないかを調べる(平行だったらreturn)
	if (Vector3Dot(v1, triangle.GetNormal()) * Vector3Dot(v2, triangle.GetNormal()) > 0) return false;

	//線の端 - ポリゴンの角
	v1 = segmentPos.v1 - triangleVertexPos.v1;
	v2 = segmentPos.v2 - triangleVertexPos.v1;

	Vector3 vec1 = segmentPos.v1 - centerPos;
	Vector3 vec2 = segmentPos.v2 - centerPos;

	//線分の両端と板ポリの距離を求める
	//0で割るのを防止するためのif
	float kyori1 = 0;
	kyori1 = abs(Vector3Dot(triangle.GetNormal(), vec1)) / triangle.GetNormal().Length();
	float kyori2 = 0;
	kyori2 = abs(Vector3Dot(triangle.GetNormal(), vec2)) / triangle.GetNormal().Length();

	//内分比
	float a = kyori1 / (kyori1 + kyori2);



	//線とポリゴンが当たっている場所を調べる
	Vector3 crossVector;//ポリゴンの角から当たってる場所の座標へのベクトル
	crossVector.x = (1 - a) * v1.x + a * v2.x;
	crossVector.y = (1 - a) * v1.y + a * v2.y;
	crossVector.z = (1 - a) * v1.z + a * v2.z;
	Vector3 crossPos = triangleVertexPos.v1 + crossVector;




	//三角形1個目の判定
	Vector3 normal1;
	normal1 = LibMath::CalcNormal(triangleVertexPos.v1, triangleVertexPos.v2, crossPos);
	Vector3 normal2;
	normal2 = LibMath::CalcNormal(triangleVertexPos.v2, triangleVertexPos.v3, crossPos);
	Vector3 normal3;
	normal3 = LibMath::CalcNormal(triangleVertexPos.v3, triangleVertexPos.v1, crossPos);

	//板ポリと法線が同じか調べる
	bool isHit = false;//板ポリと法線が同じかどうか

	//ほぼ同じだったらtrue
	if (LibMath::Difference(triangle.GetNormal().x, normal1.x, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().y, normal1.y, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().z, normal1.z, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().x, normal2.x, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().y, normal2.y, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().z, normal2.z, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().x, normal3.x, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().y, normal3.y, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().z, normal3.z, 0.0001f))
	{
		isHit = true;
	}

	//衝突位置と中心がほぼ同じだったらヒット
	if (LibMath::Difference(crossPos.x, centerPos.x, 0.001f)
		&& LibMath::Difference(crossPos.y, centerPos.y, 0.001f)
		&& LibMath::Difference(crossPos.z, centerPos.z, 0.001f)
		)
	{
		isHit = true;
	}

	if (lineSegmentCalcResult && isHit) lineSegmentCalcResult->triangleHitPos = crossPos;
	if (triangleCalcResult && isHit)triangleCalcResult->lineSegment3DHitPos = crossPos;

	return isHit;
}

bool MelLib::Collision::OBBAndRay(const OBBData& obb, const RayData& ray, RayCalcResult* rayResult)
{

	// BoxとRayの判定があるので、
	// OBBの角度 * -1の角度分レイを回転させてあげればいい?
	// そんなことないわやっぱ
	// やっぱいける?
	// OBBを中心にレイの点を回転させればいけそう
	// レイのベクトルも回転させないといけない


	// なんか回転がミスってる?

	RayData rotRay;

	// ベクトルを回転
	rotRay.SetDirection(LibMath::RotateZXYVector3(ray.GetDirection(), obb.GetAngle() * -1));
	
	// 座標も回転
	Vector3 rotRayPos = ray.GetPosition();
	rotRayPos -= obb.GetPosition();
	rotRayPos = LibMath::RotateZXYVector3(rotRayPos, obb.GetAngle() * -1);
	rotRayPos += obb.GetPosition();
	rotRay.SetPosition(rotRayPos);

	bool result = BoxAndRay(obb.GetBoxData(), rotRay, rayResult);

	if (rayResult) 
	{
		// 角度分回転
		Vector3 rotRayPos = rayResult->hitPosition;
		rotRayPos -= obb.GetPosition();
		rotRayPos = LibMath::RotateZXYVector3(rotRayPos, obb.GetAngle());
		rotRayPos += obb.GetPosition();
		rayResult->hitPosition = rotRayPos;

		rayResult->hitObjectNormal = MelLib::LibMath::RotateZXYVector3(rayResult->hitObjectNormal, obb.GetAngle());
	}

	return result;
}
