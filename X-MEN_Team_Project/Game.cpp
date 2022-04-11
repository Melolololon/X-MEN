#include "Game.h"
#include<Library.h>
#include<Input.h>

#include<ModelObject.h>

#include<Camera.h>

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


	// ���쐬
	// �f�t�H���g����1,1,1�̔��Ő��������
	// �`�󂽂����񂠂邯�Ǎ���BOX��BOARD�̂�
	// ��������ύX����\������(ShapeType3D�����Ŏw��ł���悤�ɂ���\��)
	testModel.Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// ���C���J�������擾(Get�֐��ɉ����w�肵�Ȃ��ƃ��C���J�������擾����)
	// SetRotateCriteriaPosition�ŉ�]����Z�b�g
	//(�����ݒ肾��FSP�J�����̂��߁A�����J�����̍��W�BTPS���_�̎��͂���ŃZ�b�g�������l�������_�̍��W�ɂȂ�)
	MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0,0,-2));
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

	//MelLib::SceneManager::GetInstance()->Update();

	// ���t���[��Y�����3�x��]
	testModel.SetAngle(testModel.GetAngle() + MelLib::Vector3(0,3,0));
}

void Game::Draw()
{
	//MelLib::SceneManager::GetInstance()->Draw();

	// �`��
	testModel.Draw();
}