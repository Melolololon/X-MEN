#pragma once
#include<Scene.h>


class Title:public MelLib::Scene
{


public:
	Title() {}
	~Title(){}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	MelLib::Scene* GetNextScene()override;//次のシーンの指定
};

