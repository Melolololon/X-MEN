#pragma once
#include"GameObject.h"
#include<string>

namespace EnemyStatus
{
	// 初期化用

	const MelLib::Vector3 initPlayerDir = { 0,0,0 };
	const MelLib::Vector3 initPlayerPos = { 0,0,0 };
	const float MAX_PUSH_TIME = 1;
}

class Enemy : public MelLib::GameObject
{
protected:

	float hp = 1;

	// プレイヤーの方向を保管しておく 追従に使用予定
	MelLib::Vector3 playerDir;
	// プレイヤーの場所を保管しておく　距離を保つために使用　後程取得方法が変更時に修正
	MelLib::Vector3 playerPos;
	// 押出ベクトル
	MelLib::Vector3 pushVector;
	// 押し出している時間
	float pushTime;
	// 押出フラグ　オンならPushPosition関数が実行される
	bool isPush;

protected:

	void virtual Move() {}

	// 敵同士でヒットした同士の押し出しを反映する
	void PushPosition();

public:
	Enemy() 
		:hp(1), 
		playerDir(EnemyStatus::initPlayerDir),
		playerPos(EnemyStatus::initPlayerPos),
		pushVector(MelLib::Vector3()),
		pushTime(0),
		isPush(false)
	{}
	~Enemy() {}

	void Damage(float damage);

	void Hit
	(
		const GameObject& object,
		const MelLib::ShapeType3D shapeType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjShapeType,
		const std::string& hitShapeName
	)override;

#pragma region Getter

	float GetHP() const;
	// 使う用途が思いつかないので作るだけ作ってコメントアウト
	//MelLib::Vector3 GetPlayerDir() { return playerDir};


#pragma endregion

#pragma region Setter

	void SetHP(const float& hp);
	void SetPlayerDir(const MelLib::Vector3& pos);
	// プレイヤーの位置を変数で持っておくため実装　後で消す可能性大
	void SetPlayerPos(const MelLib::Vector3& pos);

#pragma endregion


};