#pragma once
#include"Enemy.h"

namespace BarrierEnemyStatus
{
	const float MAX_HP = 200;

	const float DISTANCE_TO_PLAYER = 500;
	
	const int CHANGE_POSE_FRAME = 120;
}

class BarrierEnemy : public Enemy
{
private:

	// ボールの方向ベクトル 後程使用してモデルの向きを変更する
	MelLib::Vector3 ballDir;
	// 方向を変えるためのフレーム計測
	int ChangePoseFrameCount;

private:

	void Move() override;

	void ChangePose();

public:
	BarrierEnemy();
	~BarrierEnemy(){}

	// 更新
	void Update()override;

	//描画
	void Draw()override;

	/// <summary>
	/// 当たった時に呼ばれる関数
	/// </summary>
	/// <param name="object">衝突したオブジェクトの参照</param>
	/// <param name="shapeType">相手と衝突した自分の当たり判定の形状</param>
	/// <param name="shapeName">相手と衝突した自分の当たり判定の名前</param>
	/// <param name="hitObjShapeType">衝突したオブジェクトの当たり判定の形状</param>
	/// <param name="hitShapeName">衝突したオブジェクトの判定の名前</param>
	void Hit
	(
		const GameObject& object,
		const MelLib::ShapeType3D shapeType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjShapeType,
		const std::string& hitShapeName
	)override;


};