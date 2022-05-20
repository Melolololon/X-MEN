#include "Clear.h"
#include "GamePlay.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TestObject.h"
#include"TextWrite.h"

void Clear::Initialize()
{
	// ����������
	// �K���R���X�g���N�^�ł͂Ȃ������ɏ���������������(�݌v��̓s����)
	
	// �Ǘ��N���X�ɃI�u�W�F�N�g��ǉ�
	// ObjectManager��shared_ptr�̂ݑΉ�

	//Clear�t�H���g
	MelLib::TextWrite::CreateFontData(L"Arial", 100, "Test");

}

void Clear::Update()
{
	// �}�l�[�W���[�̍X�V
	// �I�u�W�F�N�g�̍X�V�����A���菈���A�폜�������s����
	MelLib::GameObjectManager::GetInstance()->Update();	

	// A�L�[�Ō��݂̃V�[�����I�����Ď��̃V�[����
	// ���͎��̃V�[���ɍ��Ɠ����V�[�����Z�b�g���Ă��邽�߁A�ʒu�����Z�b�g����邾��
	if (MelLib::Input::KeyTrigger(DIK_SPACE)|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))isEnd = true;
}

void Clear::Draw()
{
	//Clear�t�H���g
	MelLib::TextWrite::Draw(MelLib::Vector2(500, 300), MelLib::Color(255, 0, 0, 255), L"Clear", "Test");

	// �`��
	MelLib::GameObjectManager::GetInstance()->Draw();
}

void Clear::Finalize()
{
	// �I������

	// �S�폜
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* Clear::GetNextScene()
{
	// ���̃V�[���̃|�C���^��Ԃ�
	// �� return new Title();
	return new GamePlay();
}
