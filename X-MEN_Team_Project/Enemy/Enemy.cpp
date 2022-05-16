#include"Enemy.h"
#include "FollowEnemy.h"
#include "BarrierEnemy.h"

void Enemy::PushPosition()
{
	if (pushTime >= 1)
	{
		pushVector = MelLib::Vector3();
		isPush = false;
		pushTime = 0;
	}

	if (!isPush)return;

	// ���o�x�N�g������������肷��
	const float DIVIDE_VECTOR = 10;
	const float PER_FRAME = 1.0f / 60.0f ;

	AddPosition(pushVector / DIVIDE_VECTOR);

	pushTime += PER_FRAME * EnemyStatus::MAX_PUSH_TIME;
}

void Enemy::Damage(float damage)
{
	hp -= damage;
}

void Enemy::Hit(const GameObject& object, const MelLib::ShapeType3D shapeType, const std::string& shapeName, const MelLib::ShapeType3D hitObjShapeType, const std::string& hitShapeName)
{
	if (typeid(object) == typeid(FollowEnemy) || typeid(object) == typeid(BarrierEnemy))
	{
		pushVector = GetPosition() - object.GetPosition();
		pushVector = pushVector.Normalize();
		isPush = true;
	}
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
