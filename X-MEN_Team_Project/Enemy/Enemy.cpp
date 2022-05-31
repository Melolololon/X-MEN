#include"Enemy.h"
#include "FollowEnemy.h"
#include "BarrierEnemy.h"
#include"../Ball.h"


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
	const float PER_FRAME = 1.0f / 60.0f;

	AddPosition(pushVector / DIVIDE_VECTOR);

	pushTime += PER_FRAME * EnemyStatus::MAX_PUSH_TIME;
}

void Enemy::Damage(float damage)
{
	hp -= damage;

	// hp���Ȃ��Ȃ����Ƃ��ɊǗ��N���X����폜
	if (hp <= 0)
	{
		eraseManager = true;
	}
}

void Enemy::Hit(const GameObject& object, const MelLib::ShapeType3D shapeType, const std::string& shapeName, const MelLib::ShapeType3D hitObjShapeType, const std::string& hitShapeName)
{
	// �G���m�œ���������
	if (typeid(object) == typeid(FollowEnemy) || typeid(object) == typeid(BarrierEnemy))
	{
		pushVector = GetPosition() - object.GetPosition();
		pushVector = pushVector.Normalize();
		isPush = true;
	}

	// �{�[���Ɠ���������
	if (typeid(object) == typeid(Ball))
	{
		// �{�[���̃X�s�[�h���_���[�W��^����
		const Ball* BALL = static_cast<const Ball*>(&object);
		if (BALL->GetThrowingState() != BallState::HOLD_PLAYER) Damage(BALL->GetSpeed());
		//Damage(20);
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
	// �v���C���[�̈ʒu��ۊ�
	playerPos = pos;

}
