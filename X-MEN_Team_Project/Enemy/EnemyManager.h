#pragma once
#include<memory>
#include<time.h>

#include"../Enemy/Enemy.h"
#include"../Enemy//FollowEnemy.h"
#include"../Enemy/BarrierEnemy.h"
#include"../EnemyBarrier.h"

#include"../GameManager.h"

#include"../MyLibrary/GameObjectManager.h"

namespace EnemyManage
{
	// �ŏ��ɃX�|�[������Ǐ]����G�̐�
	const float SPAWN_FOLLOW_ENEMY_NUM = 5;
	// �ŏ��ɃX�|�[������o���A�����̓G�̐�
	const float SPAWN_BARRIER_ENEMY_NUM = 2;
	// �X�|�[������Ԋu
	const float SPAWN_TIME = 2000.0f;

	// �G���m�łԂ���Ȃ�����
	const float NOT_MOVE_DISTANCE = 8.0f;

	// �G���v���C���[����ǂꂭ�炢�̋����ŏo�����Ȃ��Ȃ邩
	const float NOT_SPAWN_DISTANCE = 20.0f;
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

	//�G���|���ꂽ�t���[����
	bool isDeadFlame = false;

	// �v���C���[�̎��ӂœG���킩�Ȃ��p
	MelLib::Vector3 playerPos;

	// �o�����̃����_���p
	static std::random_device random;
	static std::default_random_engine engine;


public:

	static EnemyManager* GetInstance();

	// �����z�u�p
	void Initialize();
	void Update();
	
	void Destroy();

	// �Z�b�^�[

	void SetPlayerPos(const MelLib::Vector3& pos);

	//�G���|���ꂽ�t���[�����̃t���O�Z�b�g
	void SetIsDeadFlame(bool isDeadFlame) { this->isDeadFlame = isDeadFlame; }

	// �Q�b�^�[

	// �G�̑�����Ԃ�
	int GetEnemyCount() const;

	//�G���|���ꂽ�t���[�����Ԃ�
	bool GetIsDeadFlame() { return isDeadFlame; }

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
	void CheckEnemyDead(T& temp,bool isBarrier);

	// �G���m�łԂ��鋗���܂ňړ����Ȃ��悤��
	void EnemyBoidsMove();
	void CalcFollowEnemyMove();
	void CalcBarrierEnemyMove();
	void CalcFollowToBarrierEnemyMove();

	void MoveFollowEnemy();
	void MoveBarrierEnemy();
};


template<typename T>
inline void EnemyManager::CheckEnemyDead(T& temp,bool isBarrier)
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

			//�{�[���ɓG�����񂾂��Ƃ�`����ׂ̃t���O
			isDeadFlame = true;

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

				if (isBarrier)
				{
					enemyBarriers[i].get()->TrueEraseManager();
					enemyBarriers.erase(enemyBarriers.begin() + i);
				}
			}
		}
	}

}
