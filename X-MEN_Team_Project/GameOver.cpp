#include "GameOver.h"
#include "GamePlay.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TestObject.h"

void GameOver::Initialize()
{
	// ����������
	// �K���R���X�g���N�^�ł͂Ȃ������ɏ���������������(�݌v��̓s����)

	// �Ǘ��N���X�ɃI�u�W�F�N�g��ǉ�
	// ObjectManager��shared_ptr�̂ݑΉ�

	//�X�v���C�g
	gameOverFont.Create(MelLib::Color(0, 0, 255, 255));
	gameOverFont.SetScale(MelLib::Vector2(600, 200));
	gameOverFont.SetPosition(MelLib::Vector2(300, 300));
	//// �e�X�g�I�u�W�F�N�g�ǉ�
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));

}

void GameOver::Update()
{
	// �}�l�[�W���[�̍X�V
	// �I�u�W�F�N�g�̍X�V�����A���菈���A�폜�������s����
	MelLib::GameObjectManager::GetInstance()->Update();

	// A�L�[�Ō��݂̃V�[�����I�����Ď��̃V�[����
	// ���͎��̃V�[���ɍ��Ɠ����V�[�����Z�b�g���Ă��邽�߁A�ʒu�����Z�b�g����邾��
	if (MelLib::Input::KeyTrigger(DIK_SPACE)|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))isEnd = true;
}

void GameOver::Draw()
{
	//�X�v���C�g
	gameOverFont.Draw();
	// �`��
	MelLib::GameObjectManager::GetInstance()->Draw();
}

void GameOver::Finalize()
{

	// �S�폜
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* GameOver::GetNextScene()
{
	// ���̃V�[���̃|�C���^��Ԃ�
	// �� return new Title();
	return new GamePlay();
}
