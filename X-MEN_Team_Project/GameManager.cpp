#include "GameManager.h"
#include <cmath>
#include <Interpolation.h>

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

void GameManager::UpdateChangeGameTime()
{
	ChangeGameTime();
}

void GameManager::Initialize()
{
	//	�ϐ��̏������p
	hitstopCount = 0;
	isHitStopNow = false;
	canGameClear = false;
	canGameOver = false;
	beforeSetGameTime = 1;
	currentSetGameTime = 1;
	gameTime = 1;
	changeGameTimeEasingValue = 0;
	isChangeGameTime = false;
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

void GameManager::SetDefaultGameTime()
{
	SetGameTime(GameManageData::DEFAULT_GAME_TIME);
}

void GameManager::SetGameTime(float setTime)
{
	// ��������ݒ�
	if (setTime > 1)setTime = 1;
	if (setTime < 0)setTime = 0;

	// �O��̃Q�[�������Ԃ��擾
	beforeSetGameTime = gameTime;
	currentSetGameTime = setTime;
	changeGameTimeEasingValue = 0;
	isChangeGameTime = true;
}

bool GameManager::GetCanGameClear() const
{
	return canGameClear;
}

bool GameManager::GetCanGameOver() const
{
	return canGameOver;
}

float GameManager::GetGameTime() const
{
	return gameTime;
}

void GameManager::ChangeGameTime()
{
	if (!isChangeGameTime)return;

	if (changeGameTimeEasingValue > 1)
	{
		gameTime = currentSetGameTime;
		changeGameTimeEasingValue = 0;
		isChangeGameTime = false;
		return;
	}

	// �C�[�W���O�̃����_��
	auto EaseOutQuint = [](float t)
	{
		if (t >= 1)return 1.0f;

		return 1.0f - std::powf(1.0f - t, 5);
	};

	gameTime = MelLib::Interpolation::Lerp(beforeSetGameTime,currentSetGameTime,EaseOutQuint(changeGameTimeEasingValue));

	const float FRAME_TIME = 1.0f / 60.0f;
	changeGameTimeEasingValue += FRAME_TIME / GameManageData::CHANGE_GAME_TIME_SEC;
}
