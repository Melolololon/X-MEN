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

	//¼aÈà¾Á½çtrue
	return LibMath::CalcDistance2D(pointPos, circlePos) <= circleR;
}

bool MelLib::Collision::PointAndCircularSector(const Vector2& pointPos, const CircularSectorData& circularSector)
{
	//_Æ~Ì»è
	if (!PointAndCircle(pointPos, circularSector.GetCircleData()))return false;

	Vector2 circlePos = circularSector.GetCircleData().GetPosition();
	float centerDirAngle = LibMath::Vector2ToAngle(circularSector.GetDirection(), false);
	float pointAngle = LibMath::Vector2ToAngle(LibMath::OtherVector2(circlePos, pointPos), false);
	float centerAngle = circularSector.GetAngle();

	//centerAngleÈà¾Á½çtrue
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

	//üÌ[©ç[ÖÌxNg
	Vector2 lineVector = segmentPos.v2 - segmentPos.v1;
	//üÌÀW1©ç~ÌS
	Vector2 linePos0ToSphere = circlePos - segmentPos.v1;
	//~ÆüÌÅZ£ðßé
	float distance = abs(Vector2::Cross(lineVector, linePos0ToSphere) / Vector2::Length(lineVector));


	//~ÆüªÌÅß_ðßé
	//±êÖ»·é

	//linePos1©çÅß_Ì£ðßé
	float linePos0ToNearPosDis = Vector2::Dot(Vector2::Normalize(lineVector), linePos0ToSphere);
	if (lineSegmentCalcResult)
	{
		lineSegmentCalcResult->nearPos = segmentPos.v1 + (Vector2::Normalize(lineVector) * linePos0ToNearPosDis);
	}

	//£ÌÙ¤ªå«©Á½çvZI¹
	if (distance > circleR)return false;

	//üÌÀW2©ç~ÌS
	Vector2 linePos2ToSphere = circlePos - segmentPos.v2;
	//³Ì©Ç¤©ð»Ê·étO
	bool linePos1ToSpherePositive = false;
	bool linePos2ToSpherePositive = false;

	//àÏðßé
	float linePos1ToSphereDot = Vector2::Dot(lineVector, linePos0ToSphere);
	float linePos2ToSphereDot = Vector2::Dot(lineVector, linePos2ToSphere);

	//³©©mF
	if (linePos1ToSphereDot >= 0)
		linePos1ToSpherePositive = true;
	if (linePos2ToSphereDot >= 0)
		linePos2ToSpherePositive = true;


	//Ç¿çà³Ü½Í¾Á½çüé
	if (linePos1ToSpherePositive
		&& linePos2ToSpherePositive
		|| !linePos1ToSpherePositive
		&& !linePos2ToSpherePositive)
	{
		//å«³ðßé
		float linePos1ToSphereLength = Vector2::Length(linePos0ToSphere);
		float linePos2ToSphereLength = Vector2::Length(linePos2ToSphere);

		//¼aæèå«©Á½ç½ÁÄÈ¢
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

	//1©2Ìmin©maxªXYZSèÌÍÍà¾Á½ç½ÁÄé
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

		//1 Xª½¢
		//2 Yª½¢
		//3 Zª½¢
		char top = 0;
		//ÎÛÖÌxNg
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
	//JvZÌÌêðg¦Îßçêé?
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

	//èÉêÔß¢ê
	Vector3 c1 = segmentPos1.v1 + s * d1;
	Vector3 c2 = segmentPos2.v1 + t * d2;

	//ß¢êÌ£ª0¾Á½ç½ÁÄ¢é(dÈÁÄ¢éAð·µÄ¢é)
	bool isHit = LibMath::CalcDistance3D(c1, c2) == 0.0f;

	if (lineSegment1CalcResult)lineSegment1CalcResult->lineSegment3DHitPos = c1;
	if (lineSegment2CalcResult)lineSegment2CalcResult->lineSegment3DHitPos = c2;
	return isHit;

}

bool Collision::CapsuleAndCapsule(const CapsuleData& capsule1, const CapsuleData& capsule2)
{
	//ñ]K.
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

	//èJvZÉêÔß¢ê
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

	//Ç±É½Á½©ðÔ·
	if (sphereCalcResult || boxCalcResult)
	{
		// ÌÇÌªÉÔÂ©Á½©Æ¢¤îñ
		BoxHitDirection hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
		if (!isHit)
		{
			sphereCalcResult->SetBoxHitDirection(hitDirection);
			boxCalcResult->boxHitDistance = hitDirection;
			return false;
		}

		//1 Xª½¢
		//2 Yª½¢
		//3 Zª½¢
		char top = 0;
		//{bNXÖÌxNg
		Vector3 sphereToVector = box.GetPosition() - sphere.GetPosition();

		if (abs(sphereToVector.x) > abs(sphereToVector.y) &&
			abs(sphereToVector.x) > box.GetSize().x / 2)
		{
			top = 1;
			if (abs(sphereToVector.z) > abs(sphereToVector.x) &&
				abs(sphereToVector.z) > box.GetSize().z / 2)
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(sphereToVector.z) > abs(sphereToVector.y) &&
				abs(sphereToVector.z) > box.GetSize().z / 2)
				top = 3;
		}

		if (top == 1)
		{
			if (sphereToVector.x >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
			}
		}
		else if (top == 2)
		{
			if (sphereToVector.y >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_UP;
			}
		}
		else if (top == 3)
		{
			if (sphereToVector.z >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
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

	// ñ]µ½²3{(XYZ)ûüÉÍÝoÄ¢é©mF·é
	// XYZÌ²ÉOBBÌñ]ðK³¹é
	// v1,v2,v3
	Vector3 angle = obb.GetAngle();
	Vector3 axisX = LibMath::RotateZXYVector3(Vector3(1, 0, 0), angle);
	Vector3 axisY = LibMath::RotateZXYVector3(Vector3(0, 1, 0), angle);
	Vector3 axisZ = LibMath::RotateZXYVector3(Vector3(0, 0, 1), angle);

	// GP
	//  ÌS©çÌSÖÌxNg
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
	
	// Èº@üvZ
	
	// @üæ¾Ç¤·é
	// lp`Ìpxð0,0,0ÌÉßµAÌÊuàßµ½ªñ]B( ÍGetBoxDataÅæ¾·êÎ¢¢ÌÅAñ]³¹éKvÍÈ¢)
	// BoxÆ¯¶æ¤Éßé
	// @üðñ]
	
	BoxHitDirection hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
	
	BoxData box = obb.GetBoxData();
	//1 Xª½¢
	//2 Yª½¢
	//3 Zª½¢
	char top = 0;

	// pxÌ}CiXªñ]
	// OBBðîÉñ]³¹È¢Æ¢¯È¢
	Vector3 rotSphere = LibMath::RotateZXYVector3(boxToSphere, -angle);
	rotSphere += box.GetPosition();

	//{bNXÖÌxNg
	Vector3 sphereToVector = box.GetPosition() - rotSphere;

	if (abs(sphereToVector.x) > abs(sphereToVector.y) &&
		abs(sphereToVector.x) > box.GetSize().x / 2)
	{
		top = 1;
		if (abs(sphereToVector.z) > abs(sphereToVector.x) &&
			abs(sphereToVector.z) > box.GetSize().z / 2)
			top = 3;
	}
	else
	{
		top = 2;
		if (abs(sphereToVector.z) > abs(sphereToVector.y) &&
			abs(sphereToVector.z) > box.GetSize().z / 2)
			top = 3;
	}

	if (top == 1)
	{
		if (sphereToVector.x >= 0)
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
		}
		else
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
		}
	}
	else if (top == 2)
	{
		if (sphereToVector.y >= 0)
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
		}
		else
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_UP;
		}
	}
	else if (top == 3)
	{
		if (sphereToVector.z >= 0)
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
		}
		else
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
		}
	}

	
	// @üðßÄZbg
	Vector3 normal = CalcHitBoxSurfaceNormal(hitDirection);
	sphereCalcResult->SetOBBHitSurfaceNormal(LibMath::RotateZXYVector3(normal, angle));
	
	return result;
}

bool Collision::SphereAndCapsule(const SphereData& sphere, const CapsuleData& capsule)
{
	Value2<Vector3>capsuleLineSegmentPos = capsule.GetSegment3DData().GetRotatePosition();
	Vector3 spherePos = sphere.GetPosition();

	Vector3 capsulePos0ToSphere = spherePos - capsuleLineSegmentPos.v1;
		//LibMath::OtherVector3(capsuleLineSegmentPos.v1, spherePos);

	//¿Ìn
	Vector3 capsuleLineSegmentVector = capsuleLineSegmentPos.v2 - capsuleLineSegmentPos.v1;
		//LibMath::OtherVector3(capsuleLineSegmentPos.v1, capsuleLineSegmentPos.v2);

	float t = Vector3::Dot(capsulePos0ToSphere, capsuleLineSegmentVector.Normalize());

	//©çüªÉ¼Éºµ½üãÌÀW ¿ÌPs¨Pn
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

bool MelLib::Collision::PlaneAndSegment3D(const PlaneData& plane, const Segment3DData& segment, Segment3DCalcResult* segmentResult)
{
	Vector3 planeToSegV1 = segment.GetPosition().v1 - plane.GetPosition();
	Vector3 planeToSegV2 = segment.GetPosition().v2 - plane.GetPosition();

	bool result = Vector3::Dot(planeToSegV1, plane.GetNormal()) * Vector3::Dot(planeToSegV2, plane.GetNormal()) <= 0;

	if (segmentResult)
	{
		MelLib::Vector3 vec1 = segment.GetPosition().v1 - plane.GetPosition();
		MelLib::Vector3 vec2 = segment.GetPosition().v2 - plane.GetPosition();

		//àªäðßAÕË_ðßé
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

	//üªÂ|ÆÀsÅÍÈ¢©ð²×é(½s¾Á½çreturn)
	if (Vector3Dot(v1, board.GetNormal()) * Vector3Dot(v2, board.GetNormal()) > 0)
		return false;


	//±±É|SÌÍÍà©Ç¤©Ìð­

	//üÌ[ - |SÌp
	v1 = segmentPos.v1 - leftDownPos;
	v2 = segmentPos.v2 - leftDownPos;

	Vector3 vec1 = segmentPos.v1 - board.GetPosition();
	Vector3 vec2 = segmentPos.v2 - board.GetPosition();

	//üªÌ¼[ÆÂ|Ì£ðßé
	//0ÅéÌðh~·é½ßÌif
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

	//àªä
	float a = kyori1 / (kyori1 + kyori2);



	//üÆ|Sª½ÁÄ¢éêð²×é
	Vector3 crossVector;//|SÌp©ç½ÁÄéêÌÀWÖÌxNg
	crossVector.x = (1 - a) * v1.x + a * v2.x;
	crossVector.y = (1 - a) * v1.y + a * v2.y;
	crossVector.z = (1 - a) * v1.z + a * v2.z;
	Vector3 crossPos = leftDownPos + crossVector;




	//Op`1ÂÚÌ»è
	Vector3 normal1;
	normal1 = LibMath::CalcNormal(leftDownPos, leftUpPos, crossPos);
	Vector3 normal2;
	normal2 = LibMath::CalcNormal(leftUpPos, rightDownPos, crossPos);
	Vector3 normal3;
	normal3 = LibMath::CalcNormal(rightDownPos, leftDownPos, crossPos);

	//Â|Æ@üª¯¶©²×é
	bool equal1 = false;//Â|Æ@üª¯¶©Ç¤©

	//ÙÚ¯¶¾Á½çtrue
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


	//Op`2ÂÚÌ»è
	normal1 = LibMath::CalcNormal(rightDownPos, leftUpPos, crossPos);
	normal2 = LibMath::CalcNormal(leftUpPos, rightUpPos, crossPos);
	normal3 = LibMath::CalcNormal(rightUpPos, rightDownPos, crossPos);

	//Â|Æ@üª¯¶©²×é
	bool equal2 = false;//Â|Æ@üª¯¶©Ç¤©

	//ÙÚ¯¶(ë·0.0001Èà)¾Á½çtrue
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

	//Ç¿ç©ª¯¶(Â|Ì)¾Á½çifÌÉ
	if (equal1 || equal2)
	{
		if (lineSegmentCalcResult) lineSegmentCalcResult->boardHitPos = crossPos;
		if (boardCalcResult)boardCalcResult->lineSegment3DHitPos = crossPos;
		return true;
	}

	//ÕËÊuÆSªÙÚ¯¶¾Á½çqbg
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
	//üªÌ»èª¬§µÄ¢éAÜ½ÍA£ª¼aÈà
	//ãÒª¬§µÄé©ð»f·êÎ¢¢

	//£ðÇ¤ßé©

	//±êÀÅ«êÎAÇÆÌ»èàÆêé

	//¼[ÌÓÌûüÉÂ|Sð¼aª®©µÄAð·µ½çA
	//JvZÆ½ÁÄéÁÄ¾¦é?
	//SÅà¢¢?
	//±Ìû@¶á_¾Á½

	//ÕË_Íð·µÄÈ­Äàßçêé?(½Êµ¢µÄvZµÄéÈçÅ«éÍ¸)
	//ÕË_ÆlÂÌÓÌ£ª¼aÈàAÜ½ÍA½ÊÆÌ£ª¼aÈà¾Á½çOK?
	//»ê¾ÆAÍÍOÅà½ÊÆð·µÄ½çAEg¾©ç_


	//¶á Alp`ÌÍÍà¾Á½çA¼[Æ½ÊÆÌ£ðvZ
	//ÍÍO¾Á½çAÓÆÌ£ðvZ
	//»ê¾ÆAJvZÆÂª½sÌAEg
	//½s¾Á½çÍÍàÅà½ÊÆÌ£ðvZ·é?
	// ÆAÎßÉÈÁÄÄÍÍOÉ¢Ä©ÂSÌß­É¢ÄÂªN\fJ©Á½çAEg
	//»ÌêA¼[Æ½ÊÌ£ðßêÎ¢¢?
	//»à»àAß¿á­¿á·©Á½çA¼[Ì£ßÄàÓ¡È¢?

	//ÍÍOÌAÓÆÌ£ðvZB»êÅ¾ß¾Á½ç¼[Ì£ðßêÎ¢¢?(ÀÝÉZ­Ä¼aÅ©¢âÂp)
	//ÍÍOÅ¼[Ì£ðßéêAÕË_ªÉ é©mFµÈ¢Æ¢¯È¢?
	//»à»àÕË_ªOÉÈéêÍAüª¯mÌ`FbNÉøÁ©©é© ½çÈ¢
	//N\·¢Ìªh³ÁÄ½çA¼[Æ½ÊÌ£ßÄàÓ¡È¢ê é©çAh³ÁÄé©Ç¤©à»fµÄA
	//h³ÁÄ½ç½ÁÄéBh³ÁÄÈ©Á½ç¼[ßêÎ¢¢B
	//½sÌAPÉ£ðßéÆA½ÁÄÈ¢ÌÉ ½ÁÄé±ÆÉÈé


	//ÌêA
	//1.h³ÁÄ¢é
	//2.¼[Æ½ÊÌ£ª¼aÈà
	//Ç¿ç©ª¬§µÄêÎ¨»ç­½ÁÄ¢é

	//OÌêA
	//1.üªÆÂÌÓÌ£ª¼aÈà
	// 
	//ÊÉ¼[Æ½ÊÌ£ðvZ·éÆAÂ©çß¿á­¿á£êÄÄà½ÊÉß©Á½ç_
	//2.


	//ïµ­l¦·¬?
	//h³ÁÄé©mFµA



	//https://www.antun.net/tips/algorithm/collision.html
	//üªÆOp`Ì£ðßêÎ¢¯»¤B
	//(lp`ÍOp`ðñÂí¹½àÌÈÌÅ)
	//©êÄéÌ2DÌâÂ¾Á½


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

	//üªÂ|ÆÀsÅÍÈ¢©ð²×é(½s¾Á½çreturn)
	if (Vector3Dot(v1, board.GetNormal()) * Vector3Dot(v2, board.GetNormal()) > 0)
	{
		//½s¾Á½êA¼[Æ½ÊÌ£ª¼aÈà¾Á½ç½ÁÄé
		float planeDir = abs(Vector3::Dot(board.GetNormal(), segmentPos.v1 - board.GetPosition())) / board.GetNormal().Length();
		if (planeDir <= capsule.GetRadius())return true;

		planeDir = abs(Vector3::Dot(board.GetNormal(), segmentPos.v2 - board.GetPosition())) / board.GetNormal().Length();
		if (planeDir <= capsule.GetRadius())return true;

		return false;
	}


	//N«½çâé±Æ
	//ÍÍà©O©ð²×Äðs¤


}

bool MelLib::Collision::PointAndSphere(const Vector3& pointPos, const SphereData& sphere)
{
	return LibMath::CalcDistance3D(pointPos, sphere.GetPosition()) <= sphere.GetRadius();
}

//
//bool MelLib::Collision::PointAndCircularSector3D(const Vector3& pointPos, const CircularSector3DData& circularSector)
//{
//	//ÌÉ é©Ç¤©mF
//	if (!PointAndSphere(pointPos, circularSector.GetSphereData()))return;
//
//	//âèû
//	//1.
//	//cÆ¡ÌpxÉ¶ÄA{[hðñ]·é?
//	//Ç¤âÁÄÉ é©mF·éÌ?
//	//\ Ì»èðs¦ÎmFÅ«é
//
//	//2.
//	//î`ð2Â½¹é?
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
	//_ªAäÌSÄÌÊÌ ¤É Á½çtrue

	std::vector<BoardData>boards = frustum.GetBoardDatas();

	for (const auto& b : boards)
	{
		//\¤É Á½çfalse
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

	// p n_ÀW
	// d ûüxNg
	// hw hh hd boxSizeHÌ x y z

	Vector3 boxSizeH = box.GetSize() / 2;
	Vector3 tMin = -FLT_MAX;
	Vector3 tMax = FLT_MAX;
	Vector3 t;

	// boxð´_É éêÅµ©ßçêÈ¢û@ÈÌÅArayð®©µÄlp`ª´_É éÆ«Æ¯¶óµÉ·é
	Vector3 rayMovePos = ray.GetPosition() - box.GetPosition();

	// @üxNgª0ÌêACÌ_ªlp`ÉüÁÄ¢é©ðmF·é
	if (ray.GetDirection().x == 0)
	{

		if (-boxSizeH.x <= rayMovePos.x && rayMovePos.x <= boxSizeH.x)
		{
			t.x = FLT_MAX;
		}
		else
		{
			t.x = -1;
			tMin.x = 0;
			tMax.x = 0;
		}
	}
	else
	{
		float num1 = (boxSizeH.x - rayMovePos.x) / ray.GetDirection().x;
		float num2 = (-boxSizeH.x - rayMovePos.x) / ray.GetDirection().x;
		if (num1 > num2)
		{
			tMin.x = num2;
			tMax.x = num1;
		}
		else
		{
			tMin.x = num1;
			tMax.x = num2;
		}
		t.x = tMax.x - tMin.x;
	}

	if (ray.GetDirection().y == 0)
	{
		if (-boxSizeH.y <= rayMovePos.y && rayMovePos.y <= boxSizeH.y)
		{
			t.y = FLT_MAX;
		}
		else
		{
			t.y = -1;
			tMin.y = 0;
			tMax.y = 0;
		}
	}
	else
	{
		float num1 = (boxSizeH.y - rayMovePos.y) / ray.GetDirection().y;
		float num2 = (-boxSizeH.y - rayMovePos.y) / ray.GetDirection().y;
		if (num1 > num2)
		{
			tMin.y = num2;
			tMax.y = num1;
		}
		else
		{
			tMin.y = num1;
			tMax.y = num2;
		}
		t.y = tMax.y - tMin.y;

	}

	if (ray.GetDirection().z == 0)
	{
		if (-boxSizeH.z <= rayMovePos.z && rayMovePos.z <= boxSizeH.z)
		{
			t.z = FLT_MAX;
		}
		else
		{
			t.z = -1;
			tMin.z = 0;
			tMax.z = 0;
		}
	}
	else
	{
		float num1 = (boxSizeH.z - rayMovePos.z) / ray.GetDirection().z;
		float num2 = (-boxSizeH.z - rayMovePos.z) / ray.GetDirection().z;
		if (num1 > num2)
		{
			tMin.z = num2;
			tMax.z = num1;
		}
		else
		{
			tMin.z = num1;
			tMax.z = num2;
		}

		t.z = tMax.z - tMin.z;
	}

	//float min = FLT_MAX;
	//
	//if (0 <= tMax.x)min = tMax.x;
	//if (0 <= tMin.x && min > tMin.x)min = tMin.x;
	//if (0 <= tMax.y && min > tMax.y)min = tMax.y;
	//if (0 <= tMin.y && min > tMin.y)min = tMin.y;
	//if (0 <= tMax.z && min > tMax.z)min = tMax.z;
	//if (0 <= tMin.z && min > tMin.z)min = tMin.z;
	//
	//// ðð½³È©Á½çfalse
	//if (min == FLT_MAX)return false;

	// VÄ
	Vector3 minVector3 = FLT_MAX;
	if (0 <= tMax.x)minVector3.x = tMax.x;
	if (0 <= tMin.x && minVector3.x > tMin.x)minVector3.x = tMin.x;
	if (0 <= tMax.y && minVector3.y > tMax.y)minVector3.y = tMax.y;
	if (0 <= tMin.y && minVector3.y > tMin.y)minVector3.y = tMin.y;
	if (0 <= tMax.z && minVector3.z > tMax.z)minVector3.z = tMax.z;
	if (0 <= tMin.z && minVector3.z > tMin.z)minVector3.z = tMin.z;


	Vector3 n = rayMovePos + minVector3 * ray.GetDirection();
	if (-boxSizeH.x <= n.x && n.x <= boxSizeH.x
		&& -boxSizeH.y <= n.y && n.y <= boxSizeH.y
		&& -boxSizeH.z <= n.z && n.z <= boxSizeH.z)
	{
		if (rayResult)
		{
			/*Vector3 hitPos;
			if (abs(tMin.x - rayMovePos.x) < abs(tMax.x - rayMovePos.x))hitPos.x = tMin.x - rayMovePos.x;
			else hitPos.x = tMax.x + rayMovePos.x;
			if (hitPos.x == FLT_MAX)hitPos.x = ray.GetPosition().x;

			if (abs(tMin.y - rayMovePos.y) < abs(tMax.y - rayMovePos.y))hitPos.y = tMin.y - rayMovePos.y;
			else hitPos.y = tMax.y + rayMovePos.y;
			if (hitPos.y == FLT_MAX)hitPos.y = ray.GetPosition().y;

			if (abs(tMin.z - rayMovePos.z) < abs(tMax.z - rayMovePos.z))hitPos.z = tMin.z - rayMovePos.z;
			else hitPos.z = tMax.z + rayMovePos.z;
			if (hitPos.z == FLT_MAX)hitPos.z = ray.GetPosition().z;

			rayResult->hitPosition = hitPos;*/
			//rayResult->hitPosition = hitPos - box.GetPosition();
			//rayResult->hitPosition = hitPos + ray.GetPosition();

			//rayResult->hitPosition = n + box.GetPosition();
		


			float minT = minVector3.x;
			if (minT > minVector3.y)minT = minVector3.y;
			if (minT > minVector3.z)minT = minVector3.z;

			rayResult->hitPosition = ray.GetPosition() + minT * ray.GetDirection();

		}

		return true;
	}
	return false;

	//return true;
}

bool MelLib::Collision::BoxAndSegment3D(const BoxData& box, const Segment3DData& segment)
{
	RayData ray;
	ray.SetPosition(segment.GetPosition().v1);
	ray.SetDirection((segment.GetPosition().v2 - segment.GetPosition().v1).Normalize());
	RayCalcResult result;

	if (!BoxAndRay(box, ray, &result))return false;

	// ÕË_ªüªÆdÈÁÄ½ç½ÁÄé»è
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

	// ñ]Æ½sÚ®ðK³¹½Op`ÌSÀWðæ¾
	Vector3 centerPos = triangle.GetRotTranceFormCenter();

	Vector3 v1;
	Vector3 v2;

	v1 = segmentPos.v1 - centerPos;
	v2 = segmentPos.v2 - centerPos;

	//üªOp`ÆÀsÅÍÈ¢©ð²×é(½s¾Á½çreturn)
	if (Vector3Dot(v1, triangle.GetNormal()) * Vector3Dot(v2, triangle.GetNormal()) > 0) return false;

	//üÌ[ - |SÌp
	v1 = segmentPos.v1 - triangleVertexPos.v1;
	v2 = segmentPos.v2 - triangleVertexPos.v1;

	Vector3 vec1 = segmentPos.v1 - centerPos;
	Vector3 vec2 = segmentPos.v2 - centerPos;

	//üªÌ¼[ÆÂ|Ì£ðßé
	//0ÅéÌðh~·é½ßÌif
	float kyori1 = 0;
	kyori1 = abs(Vector3Dot(triangle.GetNormal(), vec1)) / triangle.GetNormal().Length();
	float kyori2 = 0;
	kyori2 = abs(Vector3Dot(triangle.GetNormal(), vec2)) / triangle.GetNormal().Length();

	//àªä
	float a = kyori1 / (kyori1 + kyori2);



	//üÆ|Sª½ÁÄ¢éêð²×é
	Vector3 crossVector;//|SÌp©ç½ÁÄéêÌÀWÖÌxNg
	crossVector.x = (1 - a) * v1.x + a * v2.x;
	crossVector.y = (1 - a) * v1.y + a * v2.y;
	crossVector.z = (1 - a) * v1.z + a * v2.z;
	Vector3 crossPos = triangleVertexPos.v1 + crossVector;




	//Op`1ÂÚÌ»è
	Vector3 normal1;
	normal1 = LibMath::CalcNormal(triangleVertexPos.v1, triangleVertexPos.v2, crossPos);
	Vector3 normal2;
	normal2 = LibMath::CalcNormal(triangleVertexPos.v2, triangleVertexPos.v3, crossPos);
	Vector3 normal3;
	normal3 = LibMath::CalcNormal(triangleVertexPos.v3, triangleVertexPos.v1, crossPos);

	//Â|Æ@üª¯¶©²×é
	bool isHit = false;//Â|Æ@üª¯¶©Ç¤©

	//ÙÚ¯¶¾Á½çtrue
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

	//ÕËÊuÆSªÙÚ¯¶¾Á½çqbg
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
