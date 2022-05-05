#pragma once
#include"GameObject.h"

namespace EnemyStatus
{
	// �������p

	const MelLib::Vector3 initPlayerDir = { 0,0,0 };
	const MelLib::Vector3 initPlayerPos = { 0,0,0 };
}

class Enemy : public MelLib::GameObject
{
protected:

	float hp = 1;

	// �v���C���[�̕�����ۊǂ��Ă��� �Ǐ]�Ɏg�p�\��
	MelLib::Vector3 playerDir;
	// �v���C���[�̏ꏊ��ۊǂ��Ă����@������ۂ��߂Ɏg�p�@����擾���@���ύX���ɏC��
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