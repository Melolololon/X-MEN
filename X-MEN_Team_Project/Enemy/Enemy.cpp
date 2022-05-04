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
	// player�̈ʒu�ւ̕����x�N�g�����Ƃ�ϐ��ɃZ�b�g
	MelLib::Vector3 result = pos - GetPosition();

	// ���K�������x�N�g����ۊ�
	playerDir = result.Normalize();
}

void Enemy::SetPlayerPos(const MelLib::Vector3& pos)
{
	playerPos = pos;
}
