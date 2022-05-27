#pragma once
#include"CollisionDetectionData.h"
#include"Values.h"

//計算に使う式と、計算によって得られた情報の構造体分ける?
//まとめるとconstで渡せないし、利用者がどれ入力すればいいのかぱっと見わかりにくい

namespace MelLib
{
	//当たり判定(の計算)クラス
	class Collision
	{
	private:

		static Vector3 CalcHitBoxSurfaceNormal(BoxHitDirection dir);

	public:

#pragma region 2D

		/// <summary>
		/// 四角形同士が当たっているかを確認します。
		/// </summary>
		/// <param name="rect1">四角形</param>
		/// <param name="rect2">四角形</param>
		/// <returns>当たっていたらtrue</returns>
		static bool RectAndRect(const RectData& rect1, const RectData& rect2);

		/// <summary>
		/// 円同士が当たっているかを確認します。
		/// </summary>
		/// <param name="circle1">円</param>
		/// <param name="circle2">円</param>
		/// <returns>当たっていたらtrue</returns>
		static bool CircleAndCircle
		(
			const CircleData& circle1,
			const CircleData& circle2
		);

		static bool PointAndCircle
		(
			const Vector2& pointPos,
			const CircleData& circle
		);

		/// <summary>
		/// 点と扇形が当たっているかを確認します。
		/// </summary>
		/// <param name="pointPos"></param>
		/// <param name="circularSector"></param>
		/// <returns></returns>
		static bool PointAndCircularSector
		(
			const Vector2& pointPos,
			const CircularSectorData& circularSector
		);

		static bool RectAndCircle(const RectData& rect, const CircleData& circle);

		/// <summary>
		/// 円と2Dの線分が当たっているかを確認します。
		/// </summary>
		/// <param name="circle">円</param>
		/// <param name="lineSegment">2Dの線分</param>
		/// <returns>当たっていたらtrue</returns>
		static bool CircleAndSegment2D
		(
			const CircleData& circle,
			CircleCalcResult* circleCalcResult,
			const Segment2DData& lineSegment,
			Segment2DCalcResult* lineSegmentCalcResult
		);


#pragma endregion

#pragma region 3D
		// ○○And○○じゃなくて、○○HitCheck関数をオーバーロードしたほうがいい

		/// <summary>
		/// 球同士が当たっているかを確認します。
		/// </summary>
		/// <param name="sphere1">球</param>
		/// <param name="sphere2">球</param>
		/// <returns>当たっていたらtrue</returns>
		static bool SphereAndSphere(const SphereData& sphere1, const SphereData& sphere2);

		/// <summary>
		/// 箱同士が当たっているかを確認します。
		/// </summary>
		/// <param name="box1">箱</param>
		/// <param name="boxCalcResult1">計算結果を入れる変数のポインタ</param>
		/// <param name="box2">箱</param>
		/// <param name="boxCalcResult2">計算結果を入れる変数のポインタ</param>
		/// <returns>当たっていたらtrue</returns>
		static bool BoxAndBox(const BoxData& box1, BoxCalcResult* boxCalcResult1, const BoxData& box2, BoxCalcResult* boxCalcResult2);

		static bool Segment3DAndSegment3D
		(
			const Segment3DData& lineSegment1,
			Segment3DCalcResult* lineSegment1CalcResult,
			const Segment3DData& lineSegment2,
			Segment3DCalcResult* lineSegment2CalcResult
		);

		/// <summary>
		/// カプセル同士が当たっているかを確認します。
		/// </summary>
		/// <param name="capsule1">カプセル</param>
		/// <param name="capsule2">カプセル</param>
		/// <returns>当たっていたらtrue</returns>
		static bool CapsuleAndCapsule(const CapsuleData& capsule1, const CapsuleData& capsule2);

		/// <summary>
		/// 球と箱が当たっているかを確認します。
		/// </summary>
		/// <param name="sphere">球</param>
		/// <param name="sphereCalcResult">計算結果を入れる変数のポインタ</param>
		/// <param name="box">箱</param>
		/// <param name="boxCalcResult">計算結果を入れる変数のポインタ</param>
		/// <returns>当たっていたらtrue</returns>
		static bool SphereAndBox(const SphereData& sphere, SphereCalcResult* sphereCalcResult, const BoxData& box, BoxCalcResult* boxCalcResult);

		static bool SphereAndOBB(const SphereData& sphere, SphereCalcResult* sphereCalcResult, const OBBData& obb);

		static bool SphereAndSegmrnt3D(const SphereData& sphere, const Segment3DData& lineSegment);

		static bool SphereAndCapsule(const SphereData& sphere, const CapsuleData& capsule);

		static bool PlaneAndSegment3D(const PlaneData& plane, const Segment3DData& segment,Segment3DCalcResult* segmentResult);

		/// <summary>
		/// 板と3Dの線分が当たっているかを確認します。
		/// </summary>
		/// <param name="board">板</param>
		/// <param name="boardCalcResult">計算結果を入れる変数のポインタ</param>
		/// <param name="lineSegment">3Dの線分</param>
		/// <param name="lineSegmentCalcResult">計算結果を入れる変数のポインタ</param>
		/// <returns>当たっていたらtrue</returns>
		static bool BoardAndSegment3D
		(
			const BoardData& board,
			BoardCalcResult* boardCalcResult,
			const Segment3DData& lineSegment,
			Segment3DCalcResult* lineSegmentCalcResult
		);

		static bool BoardAndCapsule
		(
			const BoardData& board,
			BoardCalcResult* boardCalcResult,
			const CapsuleData& capsule,
			Segment3DCalcResult* lineSegmentCalcResult
		);

		static bool PointAndSphere
		(
			const Vector3& pointPos, 
			const SphereData& sphere
		);

	/*	static bool PointAndCircularSector3D
		(
			const Vector3& pointPos,
		    const CircularSector3DData& circularSector
		);*/

		static bool PointAndFrustum
		(
			const Vector3& pointPos,
			const FrustumData& frustum
		);

		static bool BoxAndRay
		(
			const BoxData& box,
			const RayData& ray,
			RayCalcResult* rayResult
		);

		static bool BoxAndSegment3D
		(
			const BoxData& box,
			const Segment3DData& segment
		);

		static bool PointAndSegment3D
		(
			const Vector3& pointPos,
			const Segment3DData& segment
		);

		static bool TriangleAndSegment3D
		(
			const TriangleData& triangle,
			TriangleCalcResult* triangleCalcResult,
			const Segment3DData& lineSegment,
			Segment3DCalcResult* lineSegmentCalcResult
		);

		static bool OBBAndRay
		(
			const OBBData& obb,
			const RayData& ray,
			RayCalcResult* rayResult
		);
#pragma endregion
	};

}