#pragma once

#include"GameObject.h"

namespace MelLib 
{
	// objectsのオブジェクトをどうマネージャーに追加する?

	class SceneEditer
	{
	private:
		SceneEditer(){}
		~SceneEditer(){}

		SceneEditer(SceneEditer& s) = delete;
		SceneEditer& operator=(SceneEditer& s) = delete;

		void Save();

		void SelectObjectUpdate();
	private:
		// 順序保持しとけばラジオボタンの値分forで回してデータにアクセスできる(intでアクセスできる)からmap
		// 順序を他の配列に保存するのもいいかも。ループする必要なくなるから早い。けどメモリ使う
		// forで取得すると参照に代入できないし、vectorのポインタは個人的に使いたくないし、forでしか参照できないから分ける
		// ラジオボタンの順番めちゃくちゃになるからmapに変更
		// オブジェクト
		std::map< std::string,std::vector<std::shared_ptr<MelLib::GameObject>>>pObjects;
		// ラジオボタンの戻り値で上のobjectsにアクセスするための配列
		std::unordered_map<int ,std::string>objectOrderDatas;
		 
		// 選択されているタイプ
		int selectType = 0;

		GameObject* pSelectObject = nullptr;

		bool isEdit = false;
		bool releaseEdit = false;
	public:
		static SceneEditer* GetInstance();

		/// <summary>
		/// オブジェクトをエディタに登録します。
		/// </summary>
		/// <param name="object">オブジェクト</param>
		/// <param name="objectType">オブジェクトの種類</param>
		void RegisterObject(const std::shared_ptr<MelLib::GameObject>& pObject,const std::string& objectType);
		
		void Update();

		void Draw();

		/// <summary>
		/// エディタのオンオフを設定します。リリースビルド時にエディタをオンにできるようにしてない場合はオンにできません。
		/// </summary>
		/// <param name="flag"></param>
		void SetEditFlag(const bool flag);
		
		/// <summary>
		/// リリースビルド時にエディタをオンにできるようにするかどうかを設定します。
		/// </summary>
		/// <param name="flag"></param>
		void SetReleaseEditFlag(const bool flag) { releaseEdit = flag; }
	};
}
