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
	EnemyBoidsMove();

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

void EnemyManager::EnemyBoidsMove()
{
	// �Ǐ]����G�̌v�Z
	CalcFollowEnemyMove();

	// �o���A�����̌v�Z
	CalcBarrierEnemyMove();

	// �o���A�ƒǏ]����G�̌v�Z
	CalcFollowToBarrierEnemyMove();

	// �G�̈ړ�
	MoveFollowEnemy();
	MoveBarrierEnemy();
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


				MelLib::Vector3 leaveVector = { 0,0,0 };
				//const float SPEED = 0.05f;

				if (distance <= EnemyManage::NOT_MOVE_DISTANCE && distance > 0)
				{
					// �Ԃ���G�Ƃ̋t�x�N�g����AddPosition����
					leaveVector = -1 * (jEnemyPosition - iEnemyPosition);
					leaveVector = leaveVector.Normalize();

				}

				// �����vector��set
				followEnemies[i].get()->AddLeaveVector(leaveVector);
				

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

				MelLib::Vector3 leaveVector = { 0,0,0 };
				//const float SPEED = 0.2f;

				if (distance <= EnemyManage::NOT_MOVE_DISTANCE && distance > 0)
				{
					// �Ԃ���G�Ƃ̋t�x�N�g����AddPosition����
					leaveVector = -1 * (jEnemyPosition - iEnemyPosition);
					leaveVector = leaveVector.Normalize();


				}

				// �����vector��set
				barrierEnemies[i].get()->AddLeaveVector(leaveVector);
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

			MelLib::Vector3 barrierLeaveVector = { 0,0,0 };
			MelLib::Vector3 followLeaveVector = { 0,0,0 };

			if (distance <= EnemyManage::NOT_MOVE_DISTANCE && distance > 0)
			{
				// �Ԃ���G�Ƃ̋t�x�N�g����AddPosition����
				barrierLeaveVector = -1 * (jEnemyPosition - iEnemyPosition);
				barrierLeaveVector = barrierLeaveVector.Normalize();

				followLeaveVector = -1 * (iEnemyPosition - jEnemyPosition);
				followLeaveVector = followLeaveVector.Normalize();


			}

			barrierEnemies[i].get()->AddLeaveVector(barrierLeaveVector);
			followEnemies[j].get()->AddLeaveVector(followLeaveVector);
		}
	}

}

void EnemyManager::MoveFollowEnemy()
{
	for (auto x : followEnemies)
	{
		// �ʏ�ړ��ƓG�Ƃ̔����ړ��̈ړ��ʂ����v���Đ��K������
		MelLib::Vector3 tempLeave = x.get()->GetLeaveVector().Normalize();

		MelLib::Vector3 moveVector = tempLeave + x.get()->GetMovedVector().Normalize();


		x.get()->AddPosition(moveVector * FollowEnemyStatus::FOLLOW_SPEED
			* GameManager::GetInstance()->GetGameTime());


		// �ړ�������LeaveVector�̒��g��������
		x.get()->SetLeaveVector({ 0,0,0 });

	}
}

void EnemyManager::MoveBarrierEnemy()
{
	for (auto x : barrierEnemies)
	{
		// �ʏ�ړ��ƓG�Ƃ̔����ړ��̈ړ��ʂ����v���Đ��K������
		MelLib::Vector3 moveVector = x.get()->GetLeaveVector().Normalize() + x.get()->GetMovedVector();

		x.get()->AddPosition(moveVector * BarrierEnemyStatus::MOVE_SPEED * GameManager::GetInstance()->GetGameTime());


		// �ړ�������LeaveVector�̒��g��������
		x.get()->SetLeaveVector({ 0,0,0 });

	}
}
