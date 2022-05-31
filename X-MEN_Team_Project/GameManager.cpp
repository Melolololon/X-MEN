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
	// ヒットストップ中ならカウントを増加させる
	if (isHitStopNow)
	{
		hitstopCount++;

		// カウントがヒットストップを止めるカウンターよりも多い時
		// ヒットストップを止めて、カウンターを0にする
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
	//	変数の初期化用
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
	// 上限下限設定
	if (setTime > 1)setTime = 1;
	if (setTime < 0)setTime = 0;

	// 前回のゲーム内時間を取得
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

	// イージングのラムダ式
	auto EaseOutQuint = [](float t)
	{
		if (t >= 1)return 1.0f;

		return 1.0f - std::powf(1.0f - t, 5);
	};

	gameTime = MelLib::Interpolation::Lerp(beforeSetGameTime,currentSetGameTime,EaseOutQuint(changeGameTimeEasingValue));

	const float FRAME_TIME = 1.0f / 60.0f;
	changeGameTimeEasingValue += FRAME_TIME / GameManageData::CHANGE_GAME_TIME_SEC;
}
