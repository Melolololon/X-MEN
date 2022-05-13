#pragma once
#include<memory>
#include<time.h>

#include"../Enemy/Enemy.h"
#include"../Enemy//FollowEnemy.h"
#include"../Enemy/BarrierEnemy.h"


namespace EnemyManage
{
	// �ŏ��ɃX�|�[�����鐔
	const float SPAWN_ENEMY_NUM = 5;
	// �X�|�[������Ԋu
	const float SPAWN_TIME = 2000.0f;
}

// �V���O���g���p�^�[��
class EnemyManager
{
private:

	static EnemyManager* instance;

	// �ǂ�������G�̔z��
	std::vector<std::shared_ptr<FollowEnemy>> followEnemies;
	// �o���A�����̓G�̔z��
	std::vector <std::shared_ptr<BarrierEnemy>> barrierEnemies;

	// ���Ԍv���p
	clock_t timerStart;

public:

	static EnemyManager* GetInstance();

	// �����z�u�p
	void Initialize();
	void Update();
	
	void Destroy();

	// �Z�b�^�[

	void SetPlayerPos(const MelLib::Vector3& pos);

private:

	EnemyManager() = default;
	~EnemyManager() = default;

	// ����A�R�s�[�̋֎~
	void operator=(const EnemyManager& obj) = delete;
	EnemyManager(const EnemyManager& obj) = delete;

	// �G�̏o��
	void PopFollowEnemyInitialize();
	// ���Ԃł̓G�̏o��
	void PopEnemyTime();

};

