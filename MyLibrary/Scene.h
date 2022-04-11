#pragma once
#include<string>

namespace MelLib 
{
	class Scene
	{

	protected:
		bool isEnd = false;

	public:
		Scene();
		virtual ~Scene();
		void FalseIsEnd();
		virtual void Initialize() = 0;//初期化
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void Finalize() = 0;//終了処理
		virtual Scene* GetNextScene() = 0;//次のシーンの指定
		bool GetIsEnd();//シーンが終わったかどうか
		void* GetPtr();

		/// <summary>
		/// 一括削除の対象になっているリソースを削除します。
		/// </summary>
		void ResourceBatchDeletion();
	};
}
