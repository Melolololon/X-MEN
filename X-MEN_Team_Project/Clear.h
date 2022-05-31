#pragma once
#include<Scene.h>

#include"Player.h"
#include "FieldObjectManager.h"

class Clear : public MelLib::Scene
{
	enum class NextScene
	{
		CLEAR,
		TITLE,
		PLAY,
	};

private:

	std::shared_ptr<Player>pPlayer;
	//�N���A�̉摜�\��
	MelLib::Sprite2D clearFont;

	NextScene nextScene = NextScene::CLEAR;

public:
	Clear() {}
	~Clear() {}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	MelLib::Scene* GetNextScene()override;//���̃V�[���̎w��
};

