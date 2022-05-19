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
	// �G�̃I�u�W�F�N�g�̐ݒu
	PopFollowEnemyInitialize();


	// �o�����Ԃ̃^�C�}�[�Z�b�g
	timerStart = clock();

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

void EnemyManager::PopFollowEnemyInitialize()
{
	std::random_device random;
	std::default_random_engine engine;
	std::uniform_int_distribution<int> dist(-10, 10);


	for (int i = 0; i < EnemyManage::SPAWN_ENEMY_NUM; i++)
	{
		std::shared_ptr<FollowEnemy> temp = std::make_shared<FollowEnemy>();
		// �����ڂ̒ǉ�
		MelLib::GameObjectManager::GetInstance()->AddObject(temp);

		// ��ňʒu�̃����_���z�u


		temp.get()->SetPosition(MelLib::Vector3(dist(engine), 0, dist(engine)));


		followEnemies.push_back(temp);
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
	}

	
}