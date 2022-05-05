#pragma once
#include<vector>
#include<unordered_map>
#include"Scene.h"

namespace MelLib 
{
	class SceneManager final
	{

	private:

		SceneManager();
		~SceneManager();

		Scene* currentScene = nullptr;
	public:

		SceneManager(const SceneManager& sceneManager) = delete;//コピーコンストラクタ
		SceneManager& operator=(const SceneManager& sceneManager) = delete;//コピー代入演算子
		static SceneManager* GetInstance();

		void SetStartScene(Scene* startScene);
		void Update();
		void Draw();
		void Finalize();


		/// <summary>
		/// シーンを切り替える処理を行います
		/// </summary>
		/// <param name="nextScene"></param>
		void ChengeScene(Scene* nextScene);


		Scene* GetCurrentScene();
		std::string GetCurrentSceneName();
	};

}