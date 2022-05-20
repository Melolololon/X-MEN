#pragma once
#include<Scene.h>

#include"Player.h"

class GameOver : public MelLib::Scene
{
public:
	GameOver() {}
	~GameOver() {}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	MelLib::Scene* GetNextScene()override;//次のシーンの指定
};

