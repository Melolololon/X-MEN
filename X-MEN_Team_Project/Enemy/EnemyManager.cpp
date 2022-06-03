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
	// ������
	enemyCount = 0;

	// �Ǐ]����G�̃I�u�W�F�N�g�̐ݒu
	PopFollowEnemyInitialize();
	// �o���A�����̓G�̃I�u�W�F�N�g�̐ݒu
	PopBarrierEnemyInitialize();

	// �o�����Ԃ̃^�C�}�[�Z�b�g
	timerStart = clock();
}

void EnemyManager::Update()
{
	// ���Ԃł̓G�̏o��
	PopEnemyTime();

	// �G���Ԃ���Ȃ��ړ�������
	EnemyMoveCancel();

	CheckEnemyDead(followEnemies,false);
	CheckEnemyDead(barrierEnemies,true);
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

	// �o���A�����̓G�Ƀv���C���[�̍��W�������Ă���
	for (auto x : barrierEnemies)
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
		//temp.get()->SetPosition(MelLib::Vector3(0, 0, 0));
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

void EnemyManager::EnemyMoveCancel()
{
	// �Ǐ]����G�̌v�Z
	CalcFollowEnemyMove();

	// �o���A�����̌v�Z
	CalcBarrierEnemyMove();

	// �o���A�ƒǏ]����G�̌v�Z
	CalcFollowToBarrierEnemyMove();
}

void EnemyManager::CalcFollowEnemyMove()
{
	for (int i = 0; i < followEnemies.size(); i++)
	{
		for (int j = 0; j < followEnemies.size(); j++)
		{
			// �����G���m�͖���
			if (i != j)
			{
				// pos���Ƃ��Ă��ċ������v�Z
				MelLib::Vector3 iEnemyPosition = followEnemies[i].get()->GetPosition();
				MelLib::Vector3 jEnemyPosition = followEnemies[j].get()->GetPosition();

				// �������v�Z
				float calcDistanceX = iEnemyPosition.x - jEnemyPosition.x;
				float calcDistanceZ = iEnemyPosition.z - jEnemyPosition.z;
				float distance = sqrt(calcDistanceX * calcDistanceX + calcDistanceZ * calcDistanceZ);

				if (distance <= EnemyManage::NOT_MOVE_DISTANCE)
				{
					// �Ԃ���G�Ƃ̋t�x�N�g����AddPosition����
					MelLib::Vector3 moveVector = -1 * (jEnemyPosition - iEnemyPosition);
					moveVector = moveVector.Normalize();

					const float SPEED = 0.3f;
					followEnemies[i].get()->AddPosition(moveVector * SPEED);
					followEnemies[i].get()->SetMoveCancel(true);
				}
				else
				{
					followEnemies[i].get()->SetMoveCancel(false);
				}

			}
		}
	}

}

void EnemyManager::CalcBarrierEnemyMove()
{
	for (int i = 0; i < barrierEnemies.size(); i++)
	{
		for (int j = 0; j < barrierEnemies.size(); j++)
		{
			if (i != j)
			{				
				// pos���Ƃ��Ă��ċ������v�Z
				MelLib::Vector3 iEnemyPosition = barrierEnemies[i].get()->GetPosition();
				MelLib::Vector3 jEnemyPosition = barrierEnemies[j].get()->GetPosition();

				// �������v�Z
				float calcDistanceX = iEnemyPosition.x - jEnemyPosition.x;
				float calcDistanceZ = iEnemyPosition.z - jEnemyPosition.z;
				float distance = sqrt(calcDistanceX * calcDistanceX + calcDistanceZ * calcDistanceZ);

				if (distance <= EnemyManage::NOT_MOVE_DISTANCE)
				{
					// �Ԃ���G�Ƃ̋t�x�N�g����AddPosition����
					MelLib::Vector3 moveVector = -1 * (jEnemyPosition - iEnemyPosition);
					moveVector = moveVector.Normalize();

					const float SPEED = 0.2f;
					barrierEnemies[i].get()->AddPosition(moveVector * SPEED);

					barrierEnemies[i].get()->SetMoveCancel(true);

				}
				else
				{
					barrierEnemies[i].get()->SetMoveCancel(false);
				}
			}
		}
	}
}

void EnemyManager::CalcFollowToBarrierEnemyMove()
{
	for (int i = 0; i < barrierEnemies.size(); i++)
	{
		for (int j = 0; j < followEnemies.size(); j++)
		{
			// pos���Ƃ��Ă��ċ������v�Z
			MelLib::Vector3 iEnemyPosition = barrierEnemies[i].get()->GetPosition();
			MelLib::Vector3 jEnemyPosition = followEnemies[j].get()->GetPosition();

			// �������v�Z
			float calcDistanceX = iEnemyPosition.x - jEnemyPosition.x;
			float calcDistanceZ = iEnemyPosition.z - jEnemyPosition.z;
			float distance = sqrt(calcDistanceX * calcDistanceX + calcDistanceZ * calcDistanceZ);

			if (distance <= EnemyManage::NOT_MOVE_DISTANCE)
			{
				// �Ԃ���G�Ƃ̋t�x�N�g����AddPosition����
				MelLib::Vector3 moveVectorBarrier = -1 * (jEnemyPosition - iEnemyPosition);
				moveVectorBarrier = moveVectorBarrier.Normalize();

				MelLib::Vector3 moveVectorFollow = -1 * (iEnemyPosition - jEnemyPosition);
			 	moveVectorFollow = moveVectorFollow.Normalize();

				const float SPEED_FOLLOW = 0.3f;
				const float SPEED_BARRIER = 0.1f;

				barrierEnemies[i].get()->AddPosition(moveVectorBarrier * SPEED_BARRIER);
				followEnemies[j].get()->AddPosition(moveVectorFollow * SPEED_FOLLOW);
				barrierEnemies[i].get()->SetMoveCancel(true);
				followEnemies[j].get()->SetMoveCancel(true);
			}
			else
			{
				barrierEnemies[i].get()->SetMoveCancel(false);
				followEnemies[j].get()->SetMoveCancel(false);
			}
		}
	}

}
