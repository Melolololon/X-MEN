#pragma once
#include"GameObject.h"
#include"GameObject2D.h"
#include"CollisionDetectionData.h"
#include"MouseCursor.h"

#include<vector>
#include<memory>

//カーソル判定用のソート作って、全体ソートなくす
//カーソル判定用配列に移してソート

//判定を2回確認するの処理多い(objAとobjBの判定求めるとき、objAだけ、objBだけHitを呼び出すのがもったいない)からどうにかする。

// shared_ptrにしてる理由
// 生ポインタを渡す処理の場合、
// インスタンスのポインタを渡す方式だと間違えてメモリ確保して渡してしまい(new 〇〇)メモリが解放されなくなってしまう可能性がある
// (newで確保してないメモリをdeleteで開放すると問題があるため、解放処理をマネージャーで行えない)
// new〇〇を渡す場合、削除時にメモリを開放しないといけないが、間違ってnewで確保してないメモリを開放してしまう可能性があるため
// 引数でどちらか指定してもらうようにしても、ミスでエラーが出る可能性がある
// エラーが出ない、メモリが解放されないという問題が起こらず、
// かつ上2つのパターン(インスタンスのポインタを渡す方式、new〇〇を渡す方式)を両立できるためshared_ptrにしている

namespace MelLib
{
	class GameObjectManager
	{
	public:
		enum ObjectSortType
		{
			//XYZの合計値でソート
			OBJECT_SORT_NONE,
			OBJECT_SORT_XYZ_SUM,
			OBJECT_SORT_X,
			OBJECT_SORT_Y,
			OBJECT_SORT_Z,
			//カメラから近い順
			OBJECT_SORT_NEAR_DISTANCE,
			//カメラから遠い順
			OBJECT_SORT_FAR_DISTANCE,
			//ソート番号でのソート
			OBJECT_SORT_SORT_NUMBER,
		};


	private:

		std::vector<std::shared_ptr<GameObject>>objects;
		//追加されたものを一時的に入れておく配列
		std::vector<std::shared_ptr<GameObject>>addObjects;

		std::vector<std::shared_ptr<GameObject2D>>object2Ds;
		std::vector<std::shared_ptr<GameObject2D>>addObject2Ds;
		//CollisionFlag checkCollision;


		//追加したフレームごとにソートするか
		ObjectSortType addObjectSort = OBJECT_SORT_NONE;
		bool addObjectSortOrderType = false;

		//カーソル判定
		bool checkMouseCollision = false;
		Vector3 cameraPosition;
		std::unique_ptr<MouseCursor>cursor;

		//データ
		Vector3 nearPos;
		Vector3 farPos;



	private:

		GameObjectManager();
		~GameObjectManager();
		/// <summary>
		/// eraseManagerがtrueかどうかを確認します。trueの場合は削除されます
		/// </summary>
		void EraseObjectCheck();


	public:
		GameObjectManager(const GameObjectManager& obj) = delete;
		GameObjectManager& operator=(const GameObjectManager& obj) = delete;

		static GameObjectManager* GetInstance();

		void Initialize();

		void Update();

		void Draw();

		void Finalize();

		///// <summary>
		///// どの判定を行うかを設定します
		///// </summary>
		///// <param name="type"></param>
		//void SetCollisionFlag3D(const CollisionFlag& type);

		/// <summary>
		/// マウスカーソルとの判定をとるか設定します
		/// </summary>
		/// <param name="flag"></param>
		void SetMouseCollisionFlag(const bool flag);


#pragma region オブジェクトの配列関係

		/// <summary>
		/// オブジェクトの配列のメモリを確保します。
		/// </summary>
		/// <param name="reserveNum"></param>
		void ReserveObjectArray(const int reserveNum);

		/// <summary>
		/// オブジェクトを追加します。
		/// </summary>
		/// <param name="object"></param>
		void AddObject(const std::shared_ptr<GameObject>& object);


		/// <summary>
		/// オブジェクトを追加します。
		/// </summary>
		/// <param name="object"></param>
		void AddObject(const std::shared_ptr<GameObject2D>& object);

		/// <summary>
		/// 追加したフレームごとにソートするかを設定します。追加しない場合、orderTypeは無視されます。
		/// </summary>
		/// <param name="sort">ソートの仕方</param>
		/// <param name="orderType">true 昇順  false 降順</param>
		void SetAddObjectSortState(const ObjectSortType& sort, const bool& orderType);


		/// <summary>
		/// オブジェクトの配列をソートします。
		/// </summary>
		/// <param name="sort">ソートの仕方</param>
		/// <param name="orderType">true 昇順  false 降順</param>
		void ObjectSort(const ObjectSortType& sort, const bool& orderType);

		/// <summary>
		/// オブジェクトマネージャーに格納されたオブジェクトの配列をconst参照で受け取ります
		/// </summary>
		/// <returns></returns>
		const std::vector<std::shared_ptr<GameObject>>& GetRefGameObject() { return objects; };

		/// <summary>
		/// オブジェクトマネージャーに格納されたオブジェクト2Dの配列をconst参照で受け取ります
		/// </summary>
		/// <returns></returns>
		const std::vector<std::shared_ptr<GameObject2D>>& GetRefGameObject2D() { return object2Ds; };

#pragma endregion



		/// <summary>
		/// 全てのオブジェクトを配列から削除します
		/// </summary>
		void AllEraseObject();

		void AllEraseObject2D();
		void AllEraseObject3D();
	};

}