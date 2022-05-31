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
	//クリアの画像表示
	MelLib::Sprite2D clearFont;

	NextScene nextScene = NextScene::CLEAR;

public:
	Clear() {}
	~Clear() {}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	MelLib::Scene* GetNextScene()override;//次のシーンの指定
};

