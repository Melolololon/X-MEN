#include"Enemy.h"

void Enemy::Damage(float damage)
{
	hp -= damage;
}

float Enemy::GetHP() const
{
	return hp;
}

void Enemy::SetHP(const float& hp)
{
	this->hp = hp;
}

void Enemy::SetPlayerDir(const MelLib::Vector3& pos)
{
	// playerの位置への方向ベクトルをとり変数にセット
	MelLib::Vector3 result = pos - GetPosition();

	// 正規化したベクトルを保管
	playerDir = result.Normalize();
}

void Enemy::SetPlayerPos(const MelLib::Vector3& pos)
{
	playerPos = pos;
}
