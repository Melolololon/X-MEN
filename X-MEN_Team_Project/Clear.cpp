#include "Clear.h"
#include "GamePlay.h"
#include"Title.h"

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
	MelLib::TextWrite::CreateFontData(L"Arial", 50, "key text");


}

void Clear::Update()
{
	// �}�l�[�W���[�̍X�V
	// �I�u�W�F�N�g�̍X�V�����A���菈���A�폜�������s����
	MelLib::GameObjectManager::GetInstance()->Update();	

	// A�L�[�Ō��݂̃V�[�����I�����Ď��̃V�[����
	// B�L�[��GamePlay��
	if (MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))nextScene = NextScene::TITLE;
	else if (MelLib::Input::KeyTrigger(DIK_V) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::B))nextScene = NextScene::PLAY;

	if (nextScene != NextScene::CLEAR)
	{
		isEnd = true;
	}
}

void Clear::Draw()
{
	//Clear�t�H���g
	MelLib::TextWrite::Draw(MelLib::Vector2(400, 300), MelLib::Color(255, 0, 0, 255), L"Clear", "Test");
	MelLib::TextWrite::Draw(MelLib::Vector2(300, 500), MelLib::Color(255, 255, 255, 255), L"SPACE/A�Ń^�C�g���ցAV/B�Ń��g���C", "key text");

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
	switch (nextScene)
	{
	case NextScene::TITLE:
		return new Title();
		break;
	case NextScene::PLAY:
		return new GamePlay();
		break;
	}
}
