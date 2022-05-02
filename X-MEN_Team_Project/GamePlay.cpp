#include "GamePlay.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TestObject.h"

void GamePlay::Initialize()
{
	// ����������
	// �K���R���X�g���N�^�ł͂Ȃ������ɏ���������������(�݌v��̓s����)

	// �I�u�W�F�N�g�̃������m��
	pPlayer = std::make_shared<Player>();
	pBall = std::make_shared<Ball>();
	pWall = std::make_shared<Wall>();

	// �Ǘ��N���X�ɃI�u�W�F�N�g��ǉ�
	// ObjectManager��shared_ptr�̂ݑΉ�
	MelLib::GameObjectManager::GetInstance()->AddObject(pPlayer);
	MelLib::GameObjectManager::GetInstance()->AddObject(pBall);
	MelLib::GameObjectManager::GetInstance()->AddObject(pWall);

	// �e�X�g�I�u�W�F�N�g�ǉ�
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));
}

void GamePlay::Update()
{
	// �}�l�[�W���[�̍X�V
	// �I�u�W�F�N�g�̍X�V�����A���菈���A�폜�������s����
	MelLib::GameObjectManager::GetInstance()->Update();

	// A�L�[�Ō��݂̃V�[�����I�����Ď��̃V�[����
	// ���͎��̃V�[���ɍ��Ɠ����V�[�����Z�b�g���Ă��邽�߁A�ʒu�����Z�b�g����邾��
	if (MelLib::Input::KeyTrigger(DIK_A))isEnd = true;
}

void GamePlay::Draw()
{
	// �`��
	MelLib::GameObjectManager::GetInstance()->Draw();
}

void GamePlay::Finalize()
{
	// �I������

	// �S�폜
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* GamePlay::GetNextScene()
{
	// ���̃V�[���̃|�C���^��Ԃ�
	// �� return new Title();
	return new GamePlay();
}
