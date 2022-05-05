#include "SceneManager.h"
#include<assert.h>
#include<typeinfo>


using namespace MelLib;

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{

}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager inst;
	return &inst;
}

void SceneManager::SetStartScene(Scene* startScene)
{

	if (!startScene) assert(0);

	currentScene = startScene;
	currentScene->Initialize();
}

void SceneManager::Update()
{

	if (!currentScene)return;
	
	if (currentScene->GetIsEnd())
	{
		//�I������
		currentScene->Finalize();
		//�ꊇ�폜�Ώۃ��\�[�X���폜
		currentScene->ResourceBatchDeletion();

		//�V�[���؂�ւ�

		//�V�[���擾
		Scene* newScene = currentScene->GetNextScene();
		//�����|�C���^�Z�b�g�h�~
		if (newScene == currentScene)assert(0);
		//�V�[�����폜
		delete currentScene;
	    //����ւ�
		currentScene = newScene;

		//������
		currentScene->FalseIsEnd();
		currentScene->Initialize();
	}

	currentScene->Update();

}

void SceneManager::Draw()
{
	if (!currentScene)return;
	currentScene->Draw();
}

void SceneManager::Finalize()
{
	if (currentScene)
	{
		currentScene->Finalize();
		delete currentScene;
	}
	
}

Scene* SceneManager::GetCurrentScene()
{
	return currentScene;
}

std::string MelLib::SceneManager::GetCurrentSceneName()
{
	std::string name = typeid(*currentScene).name();

	// �擪6����("class ")���폜
	name.erase(name.begin(), name.begin() + 6);

	return name;
}
