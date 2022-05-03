#pragma once
#include<Scene.h>

#include"Player.h"
#include "FieldObjectManager.h"

class GamePlay : public MelLib::Scene
{
private:
	
	std::shared_ptr<Player>pPlayer;
	FieldObjectManager fieldObjectManager;

public:
	GamePlay(){}
	~GamePlay(){}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	MelLib::Scene* GetNextScene()override;//次のシーンの指定
};

