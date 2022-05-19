#include "GameManager.h"

GameManager* GameManager::instance;

GameManager* GameManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new GameManager();
		instance->Initialize();
	}

	return instance;
}

bool GameManager::IsHitStop()
{
	return isHitStopNow;
}

void GameManager::Update()
{
	// �q�b�g�X�g�b�v���Ȃ�J�E���g�𑝉�������
	if (isHitStopNow)
	{
		hitstopCount++;

		// �J�E���g���q�b�g�X�g�b�v���~�߂�J�E���^�[����������
		// �q�b�g�X�g�b�v���~�߂āA�J�E���^�[��0�ɂ���
		if (hitstopCount >= GameManageData::HITSTOP_FRAME)
		{
			isHitStopNow = false;
			hitstopCount = 0;
		}
	}

}

void GameManager::Initialize()
{
	//	�ϐ��̏������p
	hitstopCount = 0;
	isHitStopNow = false;
	canGameClear = false;
	canGameOver = false;
}

void GameManager::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void GameManager::SetCanGameClear(bool flg)
{
	canGameClear = flg;
}

void GameManager::SetCanGameOver(bool flg)
{
	canGameOver = flg;
}

void GameManager::SetHitStop(bool flg)
{
	isHitStopNow = flg;
}

bool GameManager::GetCanGameClear() const
{
	return canGameClear;
}

bool GameManager::GetCanGameOver() const
{
	return canGameOver;
}
