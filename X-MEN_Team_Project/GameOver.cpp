#include "GameOver.h"
#include "GamePlay.h"
#include"Title.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TextWrite.h"
#include"TestObject.h"

void GameOver::Initialize()
{
	// ����������
	// �K���R���X�g���N�^�ł͂Ȃ������ɏ���������������(�݌v��̓s����)

	// �Ǘ��N���X�ɃI�u�W�F�N�g��ǉ�
	// ObjectManager��shared_ptr�̂ݑΉ�

	//GameOver�t�H���g
	MelLib::TextWrite::CreateFontData(L"Arial", 100, "Test");
	MelLib::TextWrite::CreateFontData(L"Arial", 50, "key text");
	
	//// �e�X�g�I�u�W�F�N�g�ǉ�
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));

}

void GameOver::Update()
{
	// �}�l�[�W���[�̍X�V
	// �I�u�W�F�N�g�̍X�V�����A���菈���A�폜�������s����
	MelLib::GameObjectManager::GetInstance()->Update();

	// A�L�[�Ō��݂̃V�[�����I�����ă^�C�g����
	// b�L�[��GamePlay�ֈڍs
	if (MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))nextScene = NextScene::TITLE;
	else if(MelLib::Input::KeyTrigger(DIK_V) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::B))nextScene = NextScene::PLAY;

	if (nextScene != NextScene::GAMEOVER)
	{
		isEnd = true;
	}
}

void GameOver::Draw()
{
	//GameOver�t�H���g
	MelLib::TextWrite::Draw(MelLib::Vector2(400,300), MelLib::Color(0,0,255,255), L"GameOver", "Test");
	MelLib::TextWrite::Draw(MelLib::Vector2(300, 500), MelLib::Color(255, 255, 255, 255), L"SPACE/A�Ń^�C�g���ցAV/B�Ń��g���C", "key text");


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
