#pragma once
#include<Scene.h>

#include"Player.h"

class GameOver : public MelLib::Scene
{
private:

	std::shared_ptr<Player>pPlayer;

public:
	GameOver() {}
	~GameOver() {}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	MelLib::Scene* GetNextScene()override;//���̃V�[���̎w��
};

