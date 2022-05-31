#include "Title.h"
#include"TextWrite.h"
#include"TestObject.h"
#include "GamePlay.h"


#include<GameObjectManager.h>
#include<Input.h>

void Title::Initialize()
{
	// ����������
	// �K���R���X�g���N�^�ł͂Ȃ������ɏ���������������(�݌v��̓s����)

	// �Ǘ��N���X�ɃI�u�W�F�N�g��ǉ�
	// ObjectManager��shared_ptr�̂ݑΉ�

	//Title �t�H���g
	MelLib::TextWrite::CreateFontData(L"Arial", 100, "Test");

	//// �e�X�g�I�u�W�F�N�g�ǉ�
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));

}

void Title::Update()
{
	// �}�l�[�W���[�̍X�V
	// �I�u�W�F�N�g�̍X�V�����A���菈���A�폜�������s����
	MelLib::GameObjectManager::GetInstance()->Update();

	// A�L�[�Ō��݂̃V�[�����I�����Ď��̃V�[����
	// ���͎��̃V�[���ɍ��Ɠ����V�[�����Z�b�g���Ă��邽�߁A�ʒu�����Z�b�g����邾��
	if (MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))isEnd = true;
}

void Title::Draw()
{
	//Title�t�H���g
	MelLib::TextWrite::Draw(MelLib::Vector2(250, 300), MelLib::Color(255, 255, 255, 255), L"�{�[���o�E���h�o�g��!!", "Test");

	// �`��
	MelLib::GameObjectManager::GetInstance()->Draw();


}

void Title::Finalize()
{

	// �S�폜
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* Title::GetNextScene()
{
	// ���̃V�[���̃|�C���^��Ԃ�
	return new GamePlay();
}
