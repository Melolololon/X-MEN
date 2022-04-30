#include "Game.h"
#include<Library.h>
#include<Input.h>

#include<ModelObject.h>

#include<Camera.h>

#include<ImguiManager.h>
#include<SceneManager.h>
#include<GameObjectManager.h>

#include"GamePlay.h"
#include "InputDeviceManager.h"

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
	// 	 
	//MelLib::GameObjectManager::GetInstance()->SetMouseCollisionFlag(false);
	// ��������s�m��
	MelLib::GameObjectManager::GetInstance()->ReserveObjectArray(100);
	// �����V�[���Z�b�g
	MelLib::SceneManager::GetInstance()->SetStartScene(new GamePlay());

	// ���쐬
	// �f�t�H���g����1,1,1�̔��Ő��������
	// �`�󂽂����񂠂邯�Ǎ���BOX��BOARD�̂�
	// ��������ύX����\������(ShapeType3D�����Ŏw��ł���悤�ɂ���\��)
	//testModel.Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// ���C���J�������擾(Get�֐��ɉ����w�肵�Ȃ��ƃ��C���J�������擾����)
	// SetRotateCriteriaPosition�ŉ�]����Z�b�g
	//(�����ݒ肾��FSP�J�����̂��߁A�����J�����̍��W�BTPS���_�̎��͂���ŃZ�b�g�������l�������_�̍��W�ɂȂ�)
	MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0,10,-10));

	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(45, 0, 0));

	// 2022/05/01 �X�Y�L�ǉ�
	InputDeviceManager::GetInstance()->Initialize();
}


void Game::Finalize()
{
	// ���C�u�����I������
	MelLib::Library::Finalize();
}
void Game::Update()
{
	// 2022/04/11 add gates
	// 2022/04/11 add GUTTYman
	// 2022/04/11 add ShunNedachi

	// 2022/05/01 �X�Y�L�ǉ�
	InputDeviceManager::GetInstance()->Update();

	MelLib::SceneManager::GetInstance()->Update();
	
#pragma region �L�[�{�[�h�ƐF�ς��̃`���[�g���A��

	//// ���t���[��Y�����3�x��]
	//testModel.SetAngle(testModel.GetAngle() + MelLib::Vector3(0, 3, 0));

	//// �J�����̑���
	//// �L�[�{�[�h�œ��͂��ĉ�]
	//if (MelLib::Input::KeyState(DIK_LEFT)) {
	//	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(0, MelLib::Camera::Get()->GetAngle().y - 2, 0));
	//}
	//if (MelLib::Input::KeyState(DIK_RIGHT)) {
	//	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(0, MelLib::Camera::Get()->GetAngle().y + 2, 0));
	//}

	//// �F�̏�Z
	//// �}�e���A���𐶐����Ċ��蓖�Ă邱�Ƃ��ł��邪�A�ȒP�ȐF�ς��Ȃ炱�����OK
	//// Color(rgb,a)
	//// 0�`255�Ŏw��
	//// ParToUChar��0%�`100%��0�`255�ɕϊ�
	////
	//testModel.SetMulColor(MelLib::Color(255,255,255, MelLib::Color::ParToUChar(90)));
#pragma endregion

}

void Game::Draw()
{
	MelLib::SceneManager::GetInstance()->Draw();

	// �`��
	//testModel.Draw();
}