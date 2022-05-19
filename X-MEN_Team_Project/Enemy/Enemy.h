#pragma once
#include"GameObject.h"
#include<string>

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

	float hp = 1;

	// �v���C���[�̕�����ۊǂ��Ă��� �Ǐ]�Ɏg�p�\��
	MelLib::Vector3 playerDir;
	// �v���C���[�̏ꏊ��ۊǂ��Ă����@������ۂ��߂Ɏg�p�@����擾���@���ύX���ɏC��
	MelLib::Vector3 playerPos;
	// ���o�x�N�g��
	MelLib::Vector3 pushVector;
	// �����o���Ă��鎞��
	float pushTime;
	// ���o�t���O�@�I���Ȃ�PushPosition�֐������s�����
	bool isPush;

protected:

	void virtual Move() {}

	// �G���m�Ńq�b�g�������m�̉����o���𔽉f����
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
	// �g���p�r���v�����Ȃ��̂ō�邾������ăR�����g�A�E�g
	//MelLib::Vector3 GetPlayerDir() { return playerDir};


#pragma endregion

#pragma region Setter

	void SetHP(const float& hp);
	void SetPlayerDir(const MelLib::Vector3& pos);
	// �v���C���[�̈ʒu��ϐ��Ŏ����Ă������ߎ����@��ŏ����\����
	void SetPlayerPos(const MelLib::Vector3& pos);

#pragma endregion


};