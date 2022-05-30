#pragma once
#include<string>

namespace MelLib 
{
	class Scene
	{

	protected:
		bool isEnd = false;

		bool isEdit = false;
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


		// 以下開発者用関数

		void Edit();
		
		bool GetIsEdit()const { return isEdit; }

		/// <summary>
		/// 一括削除の対象になっているリソースを削除します。この関数は自動で呼び出されるため、利用者が呼び出す必要はありません。
		/// </summary>
		void ResourceBatchDeletion();
	};
}
