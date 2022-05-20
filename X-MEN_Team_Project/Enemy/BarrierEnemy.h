#pragma once
#include"Enemy.h"
#include"../EnemyBarrier.h"

namespace BarrierEnemyStatus
{
	const float MAX_HP = 4;

	const float DISTANCE_TO_PLAYER = 500;

	const int CHANGE_POSE_FRAME = 120;
}

class BarrierEnemy : public Enemy
{
private:

	// ボールの方向ベクトル 後程使用してモデルの向きを変更する
	//MelLib::Vector3 ballDir;

	// バリア
	std::shared_ptr<EnemyBarrier> pBarrier;
	// 正面ベクトル
	MelLib::Vector3 frontDir;

	std::array<MelLib::Vector3, BarrierEnemyStatus::CHANGE_POSE_FRAME> ballDir;

	//バリアソート用int
	int ballCurrentNum;//最新のボールの情報を入れる配列番号
	int ballBeforeNum;//バリアに入れる時のフレームの配列番号
	bool firstCountflg;//最初、配列を入れ切るまでの関数

	std::shared_ptr<GameObject> refBallObject;
private:

	void Move() override;

	void ChangePose();

	// 指定フレーム分の配列の整理用　UPDATE前に使用
	void BallDirSort();

	// 最初のUpdate時にのみボールの参照を取得する
	void RefBallObject();

	void SetRandomPosition();
public:
	BarrierEnemy();
	~BarrierEnemy() {}

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


	// セッター
	void SetBallDir(const MelLib::Vector3& pos);
	void SetBarrier(std::shared_ptr<EnemyBarrier> barrier);

};