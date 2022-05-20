#pragma once
#include<memory>
#include<time.h>

#include"../Enemy/Enemy.h"
#include"../Enemy//FollowEnemy.h"
#include"../Enemy/BarrierEnemy.h"
#include"../EnemyBarrier.h"

#include"../GameManager.h"

namespace EnemyManage
{
	// �ŏ��ɃX�|�[������Ǐ]����G�̐�
	const float SPAWN_FOLLOW_ENEMY_NUM = 5;
	// �ŏ��ɃX�|�[������o���A�����̓G�̐�
	const float SPAWN_BARRIER_ENEMY_NUM = 1;
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
	// �o���A�����̓G�p�o���A�z��
	std::vector<std::shared_ptr<EnemyBarrier>> enemyBarriers;

	// ���Ԍv���p
	clock_t timerStart;

	// �o�����Ă���G�̑���
	int enemyCount;

public:

	static EnemyManager* GetInstance();

	// �����z�u�p
	void Initialize();
	void Update();
	
	void Destroy();

	// �Z�b�^�[

	void SetPlayerPos(const MelLib::Vector3& pos);

	// �Q�b�^�[

	// �G�̑�����Ԃ�
	int GetEnemyCount() const;

private:

	EnemyManager() = default;
	~EnemyManager() = default;

	// ����A�R�s�[�̋֎~
	void operator=(const EnemyManager& obj) = delete;
	EnemyManager(const EnemyManager& obj) = delete;

	// �Ǐ]����G�̏o��
	void PopFollowEnemyInitialize();

	// �o���A�̓G�̏o��
	void PopBarrierEnemyInitialize();

	// ���Ԃł̓G�̏o��
	void PopEnemyTime();

	// �G������ł��邩�m�F����
	template<typename T>
	void CheckEnemyDead(T& temp);
};

template<typename T>
inline void EnemyManager::CheckEnemyDead(T& temp)
{
	bool enemyDead = false;

	// �Ǐ]����G������ł���̂��m�F
	for (auto x : temp)
	{
		// hp��0�Ȃ����
		if (x.get()->GetHP() <= 0)
		{
			x.reset();
			enemyDead = true;

			enemyCount--;

			GameManager::GetInstance()->SetHitStop(true);
		}
	}

	// 
	if (enemyDead)
	{
		for (int i = 0; i < temp.size(); i++)
		{
			if (temp[i].get()->GetHP() <= 0)
			{
				temp.erase(temp.begin() + i);
			}
		}
	}

}
