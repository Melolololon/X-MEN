#pragma once
#include"GameObject.h"
#include<string>
#include "../Particle/ParticleManager.h"

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

	float hp = 10;
	//吹っ飛ぶ移動量
	MelLib::Vector3 defeatVelocity;
	//
	int defeatCount;
	//Particle
	std::shared_ptr<ParticleManager> particle;


	// プレイヤーの方向を保管しておく 追従に使用予定
	MelLib::Vector3 playerDir;
	// プレイヤーの場所を保管しておく　距離を保つために使用　後程取得方法が変更時に修正
	MelLib::Vector3 playerPos;
	// 押出ベクトル
	MelLib::Vector3 pushVector;
	//壁のめり込んだ時に押し戻す移動量
	MelLib::Vector3 pastVelocity;

	// 押し出している時間
	float pushTime;
	// 押出フラグ　オンならPushPosition関数が実行される
	bool isPush;
	//死亡フラグ
	bool isDead;

	bool moveCancel = false;
	MelLib::Vector3 movedVector;

protected:

	void virtual Move() {}

	void virtual Defeat(){}

	// 敵同士でヒットした同士の押し出しを反映する
	void PushPosition();

	void virtual FollowToPlayer(const float& moveSpeed);
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

	bool GetIsDead() const;
	// 使う用途が思いつかないので作るだけ作ってコメントアウト
	//MelLib::Vector3 GetPlayerDir() { return playerDir};


#pragma endregion

#pragma region Setter

	void SetHP(const float& hp);
	virtual void SetPlayerDir(const MelLib::Vector3& pos);
	// プレイヤーの位置を変数で持っておくため実装　後で消す可能性大
	virtual void SetPlayerPos(const MelLib::Vector3& pos);

	void SetMoveCancel(const bool& flg);
#pragma endregion


};