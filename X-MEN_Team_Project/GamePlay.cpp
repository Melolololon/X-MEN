#include "GamePlay.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TestObject.h"

void GamePlay::Initialize()
{
	// ����������
	// �K���R���X�g���N�^�ł͂Ȃ������ɏ���������������(�݌v��̓s����)
	fieldObjectManager = FieldObjectManager::GetInstance();

	// �I�u�W�F�N�g�̃������m��
	pPlayer = std::make_shared<Player>();
	pBall = std::make_shared<Ball>();
	pBall->SetPosition(MelLib::Vector3(5, 0, -5));
	barrier = std::make_shared<NormalBarrier>();

	pPlayer.get()->SetNormalBarrier(barrier);
	pPlayer.get()->SetBall(pBall);

	pFollowEnemy = std::make_shared<FollowEnemy>();
	pEnemyBarrier = std::make_shared<EnemyBarrier>();
	pEnemyBarrier.get()->OpenBarrier();
	pBarrierEnemy = std::make_shared<BarrierEnemy>();
	pBarrierEnemy.get()->SetBarrier(pEnemyBarrier);

	// �Ǘ��N���X�ɃI�u�W�F�N�g��ǉ�
	// ObjectManager��shared_ptr�̂ݑΉ�
	MelLib::GameObjectManager::GetInstance()->AddObject(pBall);
	//�{�[���͒ǉ��I�������m�ۂ��Ă���K�v���Ȃ��̂ŉ��
	pBall = nullptr;

	MelLib::GameObjectManager::GetInstance()->AddObject(pPlayer);
	//�o���A�̃e�X�g
	MelLib::GameObjectManager::GetInstance()->AddObject(barrier);

	//// �e�X�g�I�u�W�F�N�g�ǉ�
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));

	// �G�̃e�X�g
	MelLib::GameObjectManager::GetInstance()->AddObject(pFollowEnemy);
	MelLib::GameObjectManager::GetInstance()->AddObject(pBarrierEnemy);
	MelLib::GameObjectManager::GetInstance()->AddObject(pEnemyBarrier);


	fieldObjectManager->Initialize();
}

void GamePlay::Update()
{
	// �}�l�[�W���[�̍X�V
	// �I�u�W�F�N�g�̍X�V�����A���菈���A�폜�������s����
	MelLib::GameObjectManager::GetInstance()->Update();

	// �G�̃e�X�g
	pFollowEnemy.get()->SetPlayerPos(pPlayer.get()->GetPosition());
	pFollowEnemy.get()->SetPlayerDir(pPlayer.get()->GetPosition());

	//�{�[���擾(�Ƃ肠�����ŏ��Ɍ�������1��)
	for (const auto& v : MelLib::GameObjectManager::GetInstance()->GetRefGameObject()) {
		if (typeid(*v) == typeid(Ball)) {
			pBarrierEnemy.get()->SetBallDir(v->GetPosition());
			break;
		}
	}

	// A�L�[�Ō��݂̃V�[�����I�����Ď��̃V�[����
	// ���͎��̃V�[���ɍ��Ɠ����V�[�����Z�b�g���Ă��邽�߁A�ʒu�����Z�b�g����邾��
	if (MelLib::Input::KeyTrigger(DIK_I))isEnd = true;
}

void GamePlay::Draw()
{
	// �`��
	MelLib::GameObjectManager::GetInstance()->Draw();
}

void GamePlay::Finalize()
{
	// �I������
	fieldObjectManager->Finalize();

	// �S�폜
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* GamePlay::GetNextScene()
{
	// ���̃V�[���̃|�C���^��Ԃ�
	// �� return new Title();
	return new GamePlay();
}
