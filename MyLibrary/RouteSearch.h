#pragma once
#include"Vector.h"
#include"Values.h"
#include"CollisionType.h"

namespace MelLib 
{
	// A*用ノード(マス)
	struct AStarNode
	{
		using UINT = unsigned int;

		//座標
		Vector3 position;

		Vector3 size;

		//コスト
		UINT cost = 1;

		//進行不能オブジェクトと重なっているノードかどうか
		bool hitObjectNode = false;

	};


	// 今度でいいから、プレイヤーや敵の大きさを考慮して経路を計算できるようにする

	// 経路探索クラス
	class RouteSearch
	{
		using UINT = unsigned int;
	public:
#pragma region 最短経路

		///// <summary>
		///// 引数を基準にノードの座標をセットします。
		///// </summary>
		///// <param name="leftUpPos">左上座標</param>
		///// <param name="rightDownPos">右下座標</param>
		///// <param name="nodeNumX">横分割数</param>
		///// <param name="nodeNumY">縦分割数</param>
		///// <param name="nodes">ノードのvector(sizeは0でよい)</param>
		///// <param name="upPlus">上方向がプラスかどうか</param>
		//static void SetAStarNodePosition
		//(
		//	const Vector2& leftUpPos,
		//	const Vector2& rightDownPos,
		//	const UINT& nodeNumX,
		//	const UINT& nodeNumY,
		//	std::vector< std::vector<AStarNode>>& nodes,
		//	const bool upPlus
		//);

		////この関数でコストが1じゃないオブジェクトとそのコストを渡すようにする?
		///// <summary>
		///// ノードが進行不能オブジェクトにヒットしてるかを確認します。
		///// </summary>
		///// <param name="hitObjectsPos">障害物の座標のvector</param>
		///// <param name="hitObjectsSize">障害物のサイズのvector</param>
		///// <param name="nodes">ノードのvector(SetAStarNodePositionに渡した後の配列)</param>
		//static void SetAStarNodeHitObjectNodeFlag
		//(
		//	const std::vector<Vector2>& hitObjectsPos,
		//	const std::vector<Vector2>& hitObjectsSize,
		//	std::vector< std::vector<AStarNode>>& nodes
		//);

		///// <summary>
		///// 渡されたデータをもとに最短経路を求めます。
		///// </summary>
		///// <param name="startPos">スタート地点の座標</param>
		///// <param name="endPos">ゴール地点の座標</param>
		///// <param name="nodes">ノードのvector(SetAStarNodeHitObjectNodeFlagに渡した後の配列)</param>
		///// <param name="routeVector">ゴールまでのルートを格納するvector(sizeは0でよい)</param>
		///// <returns>探索が成功したかどうか</returns>
		//static bool GetAStarCalcResult
		//(
		//	const Vector2& startPos,
		//	const Vector2& endPos,
		//	std::vector< std::vector<AStarNode>>& nodes,
		//	std::vector<Vector2>& routeVector
		//);

#pragma endregion

#pragma region 新経路探索
		/// <summary>
		/// 引数を基準にノードの座標をセットします。
		/// </summary>
		/// <param name="leftUpPos">左下手前座標</param>
		/// <param name="rightDownPos">右上奥座標</param>
		/// <param name="nodeNumX">XYZの分割数割数</param>
		/// <param name="nodes">ノードのvector(sizeは0でよい)</param>
		static void SetNodePosition
		(
			const Vector3& leftDownNearPos,
			const Vector3& rightUpFarPos,
			const Value3<UINT>& nodeNum,
			std::vector<std::vector<std::vector<AStarNode>>>& nodes
		);

		//オブジェクトマネージャーで判定計算する?
		//オブジェクトマネージャーだと毎フレーム判定取っちゃうから、専用処理をこっちに組み込んだほうがいいかも
		//オブジェクトマネージャーの配列取得して計算してもいいかも
		/// <summary>
		/// ノードが進行不能オブジェクトやコストにヒットしてるかを確認します。
		/// </summary>
		/// <param name="hitObjectsBoxData">オブジェクトのデータ</param>
		/// <param name="nodes">ノードのvector(SetAStarNodePositionに渡した後の配列)</param>
		static void SetHitObjectFlag
		(
			const std::vector<BoxData>& hitObjectsBoxData,
			std::vector<std::vector<std::vector<AStarNode>>>& nodes
		);

		/// <summary>
		/// コストを設定します。コストは加算されていきます。
		/// </summary>
		/// <param name="hitObjectsBoxData"></param>
		/// <param name="costs"></param>
		/// <param name="nodes"></param>
		static void SetCost
		(
			const std::vector<BoxData>& hitObjectsBoxData,
			const std::vector<UINT>& costs,
			std::vector<std::vector<std::vector<AStarNode>>>& nodes
		);

		/// <summary>
		/// startPosからendPosまで直進可能かどうかを判定します。
		/// </summary>
		/// <param name="startPos">開始位置</param>
		/// <param name="endPos">目的地</param>
		/// <param name="nodes">ノード</param>
		/// <param name="straightDis">直進したときに通過するノードから進行不可能ノードの距離を計算して直進可能か判定するときに使用する距離</param>
		/// <param name="startToEnd">startPosからendPosへのベクトル</param>
		/// <returns></returns>
		static bool CheckStraightMove
		(
			const Vector3& startPos,
			const Vector3& endPos,
			std::vector<std::vector<std::vector<AStarNode>>>& nodes,
			float straightDis,
			Vector3& startToEnd
		);

		/// <summary>
		/// 渡されたデータをもとに最短経路を計算します。
		/// </summary>
		/// <param name="startPos">スタート地点の座標</param>
		/// <param name="endPos">ゴール地点の座標</param>
		/// <param name="nodes">ノードのvector(SetAStarNodeHitObjectNodeFlagに渡した後の配列)</param>
		/// <param name="routeVector">ゴールまでのルートを格納するvector(sizeは0でよい)</param>
		/// <returns>探索が成功したかどうか</returns>
		static bool CalcRoute
		(
			const Vector3& startPos,
			const Vector3& endPos,
			std::vector<std::vector<std::vector<AStarNode>>>& nodes,
			std::vector<Vector3>* routeVector,
			std::vector<Vector3>* routeNodePos
		);
#pragma endregion


	};
}

