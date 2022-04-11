#include "Collision.h"
#include"LibMath.h"
#include"Values.h"

using namespace MelLib;



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
			sphereCalcResult->boxHitDistance = hitDirection;
			boxCalcResult->boxHitDistance = hitDirection;
			return false;
		}

		//1 Xが多い
		//2 Yが多い
		//3 Zが多い
		char top = 0;
		//ボックスへのベクトル
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

		if (sphereCalcResult)sphereCalcResult->boxHitDistance = hitDirection;
		if (boxCalcResult)boxCalcResult->boxHitDistance = hitDirection;
	}

	return isHit;
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

	// p 始点座標
	// d 方向ベクトル
	// hw hh hd boxSizeHの x y z

	Vector3 boxSizeH = box.GetSize() / 2;
	Vector3 tMin = -FLT_MAX;
	Vector3 tMax = FLT_MAX;
	Vector3 t;

	// boxを原点にある場合でしか求められない方法なので、rayを動かして四角形が原点にあるときと同じ状況にする
	Vector3 rayMovePos = ray.GetPosition() - box.GetPosition();

	// 法線ベクトルが0の場合、レイの点が四角形に入っているかを確認する
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
	//// 条件を満たさなかったらfalse
	//if (min == FLT_MAX)return false;

	// 新案
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
