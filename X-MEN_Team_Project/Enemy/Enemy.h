#pragma once
#include"GameObject.h"

namespace EnemyStatus
{
	// 初期化用

	const MelLib::Vector3 initPlayerDir = { 0,0,0 };
	const MelLib::Vector3 initPlayerPos = { 0,0,0 };
}

class Enemy : public MelLib::GameObject
{
protected:

	float hp = 1;

	// プレイヤーの方向を保管しておく 追従に使用予定
	MelLib::Vector3 playerDir;
	// プレイヤーの場所を保管しておく　距離を保つために使用　後程取得方法が変更時に修正
	MelLib::Vector3 playerPos;

protected:

	void virtual Move() {}

public:
	Enemy():hp(1),playerDir(EnemyStatus::initPlayerDir),
		    playerPos(EnemyStatus::initPlayerPos){}
	~Enemy(){}

	void Damage(float damage);



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