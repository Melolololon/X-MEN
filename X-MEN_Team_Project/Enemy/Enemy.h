#pragma once
#include"GameObject.h"
#include<string>
#include "../Particle/ParticleManager.h"

namespace EnemyStatus
{
	// �������p

	const MelLib::Vector3 initPlayerDir = { 0,0,0 };
	const MelLib::Vector3 initPlayerPos = { 0,0,0 };
	const float MAX_PUSH_TIME = 1;
}

class Enemy : public MelLib::GameObject
{
protected:

	float hp = 10;
	//������Ԉړ���
	MelLib::Vector3 defeatVelocity;
	//
	int defeatCount;
	//Particle
	std::shared_ptr<ParticleManager> particle;


	// �v���C���[�̕�����ۊǂ��Ă��� �Ǐ]�Ɏg�p�\��
	MelLib::Vector3 playerDir;
	// �v���C���[�̏ꏊ��ۊǂ��Ă����@������ۂ��߂Ɏg�p�@����擾���@���ύX���ɏC��
	MelLib::Vector3 playerPos;
	// ���o�x�N�g��
	MelLib::Vector3 pushVector;
	//�ǂ̂߂荞�񂾎��ɉ����߂��ړ���
	MelLib::Vector3 pastVelocity;

	// �����o���Ă��鎞��
	float pushTime;
	// ���o�t���O�@�I���Ȃ�PushPosition�֐������s�����
	bool isPush;
	//���S�t���O
	bool isDead;

	bool moveCancel = false;
	MelLib::Vector3 movedVector;

protected:

	void virtual Move() {}

	void virtual Defeat(){}

	// �G���m�Ńq�b�g�������m�̉����o���𔽉f����
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
	// �g���p�r���v�����Ȃ��̂ō�邾������ăR�����g�A�E�g
	//MelLib::Vector3 GetPlayerDir() { return playerDir};


#pragma endregion

#pragma region Setter

	void SetHP(const float& hp);
	virtual void SetPlayerDir(const MelLib::Vector3& pos);
	// �v���C���[�̈ʒu��ϐ��Ŏ����Ă������ߎ����@��ŏ����\����
	virtual void SetPlayerPos(const MelLib::Vector3& pos);

	void SetMoveCancel(const bool& flg);
#pragma endregion


};