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

void GameManager::Initialize()
{
	//	変数の初期化用
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
