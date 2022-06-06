#include "GamePlay.h"
#include"Clear.h"
#include"GameOver.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TestObject.h"

#include"Enemy/EnemyManager.h"

#include"Particle/PopUpParticleManager.h"

#include"GameManager.h"

void GamePlay::Initialize()
{
	MelLib::ModelData::Load("Resources/Model/Dome/dome.obj", true, "domeObj");
	// ����������
	// �K���R���X�g���N�^�ł͂Ȃ������ɏ���������������(�݌v��̓s����)
	fieldObjectManager = FieldObjectManager::GetInstance();

	// �I�u�W�F�N�g�̃������m��
	pPlayer = std::make_shared<Player>();
	pBall = std::make_shared<Ball>();
	//pBall->SetPosition(MelLib::Vector3(5, 0, -10));
	barrier = std::make_shared<NormalBarrier>();

	pPlayer.get()->SetNormalBarrier(barrier);
	pPlayer.get()->SetBall(pBall);

	//pFollowEnemy = std::make_shared<FollowEnemy>();
	//pEnemyBarrier = std::make_shared<EnemyBarrier>();
	//pEnemyBarrier.get()->OpenBarrier();
	//pBarrierEnemy = std::make_shared<BarrierEnemy>();
	//pBarrierEnemy.get()->SetBarrier(pEnemyBarrier);

	// �Ǘ��N���X�ɃI�u�W�F�N�g��ǉ�
	// ObjectManager��shared_ptr�̂ݑΉ�
	MelLib::GameObjectManager::GetInstance()->AddObject(pBall);

	MelLib::GameObjectManager::GetInstance()->AddObject(pPlayer);
	//�o���A�̃e�X�g
	MelLib::GameObjectManager::GetInstance()->AddObject(barrier);

	//// �e�X�g�I�u�W�F�N�g�ǉ�
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));

	// �G�̃e�X�g
	//MelLib::GameObjectManager::GetInstance()->AddObject(pFollowEnemy);
	//MelLib::GameObjectManager::GetInstance()->AddObject(pBarrierEnemy);
	//MelLib::GameObjectManager::GetInstance()->AddObject(pEnemyBarrier);

	

	//�|�b�v�A�b�v�p�[�e�B�N��
	PopUpParticleManager::GetInstance()->Initialize();

	EnemyManager::GetInstance()->Initialize();

	fieldObjectManager->Initialize();

	std::shared_ptr<UltimateSkill> ultimateSkill = std::make_shared<UltimateSkill>();
	dome = std::make_shared<Dome>();
	MelLib::GameObjectManager::GetInstance()->AddObject(dome);
	ultimateSkill.get()->SetDome(dome);
	pPlayer.get()->SetUltimateSkill(ultimateSkill);

	// GameManager�̃e�X�g
	GameManager::GetInstance()->Initialize();

	nextScene = NextScene::PLAY;
}

void GamePlay::Update()
{

	if (!GameManager::GetInstance()->IsHitStop())
	{
		// �}�l�[�W���[�̍X�V
		// �I�u�W�F�N�g�̍X�V�����A���菈���A�폜�������s����
		MelLib::GameObjectManager::GetInstance()->Update();

		// �K�E�Z���g�������ƂɃQ�[�������Ԃ��������ɂ���
		if (dome.get()->IsEndTrigger())GameManager::GetInstance()->SetGameTime(0.1f);
		if (dome.get()->IsPostProcessEndTrigger())GameManager::GetInstance()->SetDefaultGameTime();

		// �G�̃e�X�g
		//pFollowEnemy.get()->SetPlayerPos(pPlayer.get()->GetPosition());
		//pFollowEnemy.get()->SetPlayerDir(pPlayer.get()->GetPosition());

		EnemyManager::GetInstance()->SetPlayerPos(pPlayer.get()->GetPosition());
		EnemyManager::GetInstance()->Update();

		//�{�[���擾(�Ƃ肠�����ŏ��Ɍ�������1��)
		//for (const auto& v : MelLib::GameObjectManager::GetInstance()->GetRefGameObject()) {
		//	if (typeid(*v) == typeid(Ball)) {
		//		pBarrierEnemy.get()->SetBallDir(v->GetPosition());
		//		break;
		//	}
		//}

		//�|�b�v�p�[�e�B�N��
		PopUpParticleManager::GetInstance()->Update();
		if (MelLib::Input::KeyTrigger(DIK_6))
		{
			PopUpParticleManager::GetInstance()->SetFire(MelLib::Vector3());

		}

		// �m�F�p
		if (MelLib::Input::KeyTrigger(DIK_0)) GameManager::GetInstance()->SetHitStop(true);

		// �Q�[���N���A���� 
		// �G�̑�����0�ȉ��ɂȂ����Ƃ���
		if (EnemyManager::GetInstance()->GetEnemyCount() <= 0)GameManager::GetInstance()->SetCanGameClear(true);
		
		// �Q�[���I�[�o�[����
		if (pPlayer.get()->GetHp() <= 0)GameManager::GetInstance()->SetCanGameOver(true);

		GameManager::GetInstance()->UpdateChangeGameTime();
	}
	else
	{
		GameManager::GetInstance()->Update();
	}


	// A�L�[�Ō��݂̃V�[�����I�����Ď��̃V�[����
	// ���͎��̃V�[���ɍ��Ɠ����V�[�����Z�b�g���Ă��邽�߁A�ʒu�����Z�b�g����邾��
	//if (MelLib::Input::KeyTrigger(DIK_I))isEnd = true;
	if (MelLib::Input::KeyTrigger(DIK_L))nextScene = NextScene::CLEAR;
	if (MelLib::Input::KeyTrigger(DIK_P) || pPlayer.get()->GetHp() <= 0)nextScene = NextScene::GAMEOVER;

	if (GameManager::GetInstance()->GetCanGameClear())nextScene = NextScene::CLEAR;
	else if (GameManager::GetInstance()->GetCanGameOver())nextScene = NextScene::GAMEOVER;


	if (nextScene != NextScene::PLAY)
	{
		isEnd = true;
	}
}

void GamePlay::Draw()
{
	// �`��
	MelLib::GameObjectManager::GetInstance()->Draw();
	//�|�b�v�A�b�v�p�[�e�B�N��
	PopUpParticleManager::GetInstance()->Draw();
	
	// �������Ȃ̂ŕ`��
	pBall->DrawTrajectories();

}

void GamePlay::Finalize()
{
	// �I������
	fieldObjectManager->Finalize();

	// �S�폜
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();

	EnemyManager::GetInstance()->Destroy();
}

MelLib::Scene* GamePlay::GetNextScene()
{
	// ���̃V�[���̃|�C���^��Ԃ�
	switch (nextScene)
	{
	case NextScene::CLEAR:
		return new Clear();
		nextScene = NextScene::PLAY;
		break;
	case NextScene::GAMEOVER:
		return new GameOver();
		nextScene = NextScene::PLAY;
		break;
	}
}
