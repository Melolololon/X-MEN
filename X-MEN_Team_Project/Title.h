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
	void Finalize()override;//�I������
	MelLib::Scene* GetNextScene()override;//���̃V�[���̎w��
};

