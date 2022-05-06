#pragma once
#include<Scene.h>

#include"Player.h"
#include"NormalBarrier.h"
#include "FieldObjectManager.h"
#include "Ball.h"
#include "Wall.h"

class GamePlay : public MelLib::Scene
{
private:
	
	std::shared_ptr<Player>pPlayer;
	std::shared_ptr<NormalBarrier>barrier;
	FieldObjectManager* fieldObjectManager;
	std::shared_ptr<Ball> pBall;
	std::shared_ptr<Wall> pWall;

public:
	GamePlay(){}
	~GamePlay(){}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	MelLib::Scene* GetNextScene()override;//���̃V�[���̎w��
};

