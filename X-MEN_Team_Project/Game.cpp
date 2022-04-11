#include "Game.h"
#include<Library.h>
#include<Input.h>

#include<ImguiManager.h>



Game::Game() {}

Game::~Game() {}

Game* Game::GetInstance()
{
	static Game game;
	return &game;
}


void Game::Run()
{
	// �����[�X�r���h����ImGui��`�悷�邩�w��
	MelLib::ImguiManager::GetInstance()->SetReleaseDrawFlag(true);

	// ������
	Initialize();

	// �Q�[�����[�v(����������K�v�Ȃ�!)
	while (1)
	{
		MelLib::Library::LoopStartProcess();

		if (MelLib::Input::KeyTrigger(DIK_ESCAPE))MelLib::Library::EndFlagTrue();
		if (MelLib::Library::GetIsEnd())break;


		Update();
		Draw();
		MelLib::Library::LoopEndProcess();

	}

	// �I��
	Finalize();
}

void Game::Initialize()
{
	// ���C�u����������(��,�c,�F,���O)
	MelLib::Library::Initialize(1280, 720, MelLib::Color(30, 30, 160, 255), L"Game");
	// 60�t���[���Œ�
	MelLib::Library::SetFramesPerSecond60(true);

	// �}�l�[�W���[�̐ݒ�
	//MelLib::GameObjectManager::GetInstance()->SetMouseCollisionFlag(false);
	//MelLib::GameObjectManager::GetInstance()->ReserveObjectArray(100);
	//MelLib::SceneManager::GetInstance()->SetStartScene(new Play());

}


void Game::Finalize()
{
	// ���C�u�����I������
	MelLib::Library::Finalize();
}
void Game::Update()
{
	// 2022/04/11 add gates

	//MelLib::SceneManager::GetInstance()->Update();
}

void Game::Draw()
{
	//MelLib::SceneManager::GetInstance()->Draw();
}