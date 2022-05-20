#include "EnemyManager.h"
#include<GameObjectManager.h>
#include<memory>
#include<time.h>
#include<random>

// �ÓI�Ȏ���
EnemyManager* EnemyManager::instance;

EnemyManager* EnemyManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new EnemyManager();
	}
	
	return instance;
}

void EnemyManager::Initialize()
{
	// �Ǐ]����G�̃I�u�W�F�N�g�̐ݒu
	PopFollowEnemyInitialize();
	// �o���A�����̓G�̃I�u�W�F�N�g�̐ݒu
	PopBarrierEnemyInitialize();

	// �o�����Ԃ̃^�C�}�[�Z�b�g
	timerStart = clock();

	// ������
	enemyCount = 0;
}

void EnemyManager::Update()
{
	// ���Ԃł̓G�̏o��
	PopEnemyTime();

	CheckEnemyDead(followEnemies);
	CheckEnemyDead(barrierEnemies);
}

void EnemyManager::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}

}

void EnemyManager::SetPlayerPos(const MelLib::Vector3& pos)
{
	// �Ǐ]����G�Ƀv���C���[�̍��W�������Ă���
	for (auto x : followEnemies)
	{
		x.get()->SetPlayerDir(pos);
		x.get()->SetPlayerPos(pos);
	}
}

int EnemyManager::GetEnemyCount() const
{
	return enemyCount;
}

void EnemyManager::PopFollowEnemyInitialize()
{
	std::random_device random;
	std::default_random_engine engine;
	std::uniform_int_distribution<int> dist(-10, 10);


	for (int i = 0; i < EnemyManage::SPAWN_FOLLOW_ENEMY_NUM; i++)
	{
		std::shared_ptr<FollowEnemy> temp = std::make_shared<FollowEnemy>();
		// �����ڂ̒ǉ�
		MelLib::GameObjectManager::GetInstance()->AddObject(temp);

		// ��ňʒu�̃����_���z�u


		temp.get()->SetPosition(MelLib::Vector3(dist(engine), 0, dist(engine)));


		followEnemies.push_back(temp);

		enemyCount++;
	}
}

void EnemyManager::PopBarrierEnemyInitialize()
{

	// �o���A�����̓G�𐶐�
	for (int i = 0; i < EnemyManage::SPAWN_BARRIER_ENEMY_NUM; i++)
	{
		std::shared_ptr<BarrierEnemy> temp = std::make_shared<BarrierEnemy>();

		std::shared_ptr<EnemyBarrier> tempBarrier = std::make_shared<EnemyBarrier>();

		// �o���A�ݒ�
		tempBarrier.get()->OpenBarrier();

		// �����ڂ̒ǉ�
		MelLib::GameObjectManager::GetInstance()->AddObject(temp);
		MelLib::GameObjectManager::GetInstance()->AddObject(tempBarrier);



		// �Ƃ肠����0,0,0�ɐݒu
		temp.get()->SetPosition(MelLib::Vector3(0, 0, 0));
		temp.get()->SetBarrier(tempBarrier);

		barrierEnemies.push_back(temp);
		enemyBarriers.push_back(tempBarrier);

		enemyCount++;
	}
}

void EnemyManager::PopEnemyTime()
{
	// ���b����������G�̒ǉ�
	// �o�ߎ��Ԃ̌v��
	clock_t timerEnd = clock(); 
	

	double time = (double)(timerEnd - timerStart) / CLOCKS_PER_SEC;

	// �ݒ肵�����Ԃ����o�ߎ��Ԃ�����������
	if (time > EnemyManage::SPAWN_TIME)
	{

		std::shared_ptr<FollowEnemy> followEnemy = std::make_shared<FollowEnemy>();

		//�@�����ڂ̒ǉ�
		MelLib::GameObjectManager::GetInstance()->AddObject(followEnemy);


		followEnemies.push_back(followEnemy);

		timerStart = clock();

		enemyCount++;
	}

	
}