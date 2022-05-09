#pragma once
#include<Scene.h>

#include"Player.h"
#include"NormalBarrier.h"
#include "FieldObjectManager.h"
#include "Ball.h"
#include"Enemy/FollowEnemy.h"
#include"Enemy/BarrierEnemy.h"

class GamePlay : public MelLib::Scene
{
private:
	
	std::shared_ptr<Player>pPlayer;
	std::shared_ptr<NormalBarrier>barrier;
	FieldObjectManager* fieldObjectManager;
	// 敵
	std::shared_ptr<FollowEnemy> pFollowEnemy;
	std::shared_ptr<BarrierEnemy> pBarrierEnemy;

public:
	GamePlay(){}
	~GamePlay(){}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	MelLib::Scene* GetNextScene()override;//次のシーンの指定
};

