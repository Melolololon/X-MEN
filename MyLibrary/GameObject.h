#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionType.h"
#include"Physics.h"

#include"ModelObject.h"

#ifdef _DEBUG
#include"PipelineState.h"
#include"Material.h"
#endif // _DEBUG


//オブジェクトマネージャー追加時に判定を選ぶようにする?(判定ごとに追加関数と配列作る)

//GameObjectを継承させてPhysicsObject作ると、Hit関数で重さとか受け取れなくなるからまとめた

//velocityとかを勝手に書き換えられるとバグる。
//GameObjectのpositionとかをprivateにして関数でセットするようにする?(velocityは書き換えられないようにする)
//そうすれば、Unityみたいになる
//positionもvelocityもセットできるようにする。(変数用意するのめんどいから)


//アクションRPGとかで当たったアイテムに称号とかついてたり、取得できるゴールドが不特定の場合、
//管理クラス作って、アイテム出現時にそれに追加して、
//Hit関数のObjectとアイテムのポインタが一致してたら、アイテム出現時に追加したポインタ全部と比較して、
//アドレスが同じポインタもらってパラメータ受け取ればいい
//プレイヤーのポインタ渡してお金側でセットしてもいい

namespace MelLib 
{
	class GameObject
	{
	private:

#ifdef _DEBUG
		//判定確認用モデルのパイプライン
		static PipelineState collisionCheckModelPipelineState;

		//判定確認用モデルのマテリアル
		static ADSAMaterial material;

		//判定確認用モデル

		std::vector<ModelObject>sphereModelObjects;
		std::vector<ModelObject>boxModelObjects;
		std::vector<ModelObject>boardModelObjects;
		std::array<std::vector<ModelObject>, 2>segmentModelObjects;

		//カプセル引き伸ばすと形崩れるから、球と円柱組み合わせて表示したほうがいいかも
		//判定のデータの変数は今のまま(角度消すと円柱を組み合わせるのが大変になる)
		std::array<std::vector<ModelObject>, 2>capsuleModelObjects;


#endif // _DEBUG

		Vector3 position = 0;
		Vector3 angle = 0;
		Vector3 scale = 0;

#pragma region 物理関係
		
		//物体が動く力
		Vector3 force = 0;
		//重さ
		float mass = 1.0f;
		//速度
		Vector3 velocity = 0;
		//重力加速度
		static float gravutationalAcc;
		//加速度
		Vector3 acceleration = 0;

		//落下時間
		int fallTime = 0;
		//落下初速
		float fallStartSpeed = 0.0f;
		//現在の落下速度
		float currentFallVelovity = 0.0f;
		//落下しているかどうか
		bool isFall = false;
#pragma endregion

		//当たった相手のデータ
		SphereData hitSphereData;
		BoxData hitBoxData;
		Segment3DData hitSegment3DData;
		RayData hitLayData;
		PlaneData hitPlaneData;
		BoardData hitBoardData;
		CapsuleData hitCapsuleData;
		TriangleData hitTriangleData;
	protected:

#pragma region 判定データ
		CollisionDetectionFlag collisionFlag;

		std::vector<SphereData> sphereData;
		std::vector<BoxData> boxData;
		std::vector<Segment3DData> segment3DData;
		std::vector<RayData> layData;
		std::vector<PlaneData>planeData;
		std::vector<BoardData>boardData;
		std::vector<CapsuleData>capsuleData;
		std::vector<TriangleData>triangleData;


#pragma endregion
		
		//継承したクラスを格納し、判定時に使う用
		std::vector<std::string>tags;

		std::unordered_map<std::string, ModelObject>modelObjects;

		//ソート用数値。ソート順を自分で調整するための変数
		short sortNumber = 0;

		//生死フラグ(これがtrueになると、オブジェクトマネージャーから除外される)
		bool eraseManager = false;
		

		bool drawCollisionModel = true;

	protected:
		SphereData GetHitSphereData() const { return hitSphereData; }
		BoxData GetHitBoxData() const { return hitBoxData; }
		Segment3DData GetHitSegmentData() const { return hitSegment3DData; }
		PlaneData GetHitPlaneData() const { return hitPlaneData; }
		BoardData GetHitBoardData()const { return hitBoardData; }
		CapsuleData GetHitCapsuleData() const { return hitCapsuleData; }
		TriangleData GetHitTriangleData() const { return hitTriangleData; }



#pragma region 物理演算
		//反発とかもHit関数で自分で呼ぶようにする?
		//名前PhysicsMoveに変える?
		//CalcMovePhysicsで重力加速度計算しないようにする?

		//もう物理演算まとめる?
		//CalcThrowUpPhysicsとか引数いるやつはまとめられない

		//物理演算行ったらマネージャーで座標移動させる?
		//移動量計算すればちゃんと移動できる

		/// <summary>
		/// 座標に関する物理演算を行います。
		/// </summary>
		void CalcMovePhysics();



		/// <summary>
		/// 落下処理を開始します。
		/// </summary>
		/// <param name="startSpeed">初速度</param>
		void FallStart(const float startSpeed)
		{
			this->fallStartSpeed = startSpeed;
			isFall = true;
		}

		/// <summary>
		/// 落下処理を終了します。
		/// </summary>
		void FallEnd()
		{
			fallTime = 0;
			fallStartSpeed = 0.0f;
			isFall = false;

			//引いて投げ上げによる加速度をとりあえず引く
			//本当は引かずに反発の処理やったほうがいい
			velocity.y -= currentFallVelovity;


			currentFallVelovity = 0.0f;
		}



#pragma endregion
	private:
		/// <summary>
		/// モデルの移動
		/// </summary>
		/// <param name="vec">移動量</param>
		void SetModelPosition(const Vector3& vec);

		/// <summary>
		/// 判定の移動
		/// </summary>
		/// <param name="vec"></param>
		void SetDataPosition(const Vector3& vec);

		void SetModelAngle(const Vector3& angle);

		void SetDataAngle(const Vector3& angle);

		void SetModelScale(const Vector3& scale);

		void SetDataScale(const Vector3& scale);

	public:

		//コンストラクタ
		GameObject();
		//デストラクタ
		virtual ~GameObject();

		//更新
		virtual void Update();
		//描画
		virtual void Draw();

		/// <summary>
		/// 当たった時の処理
		/// </summary>
		/// <param name="object">相手オブジェトのポインタ</param>
		/// <param name="collisionType">自分のどの判定に当たったか</param>
		/// <param name="arrayNum">自分の何個目の判定に当たったか</param>
		/// <param name="hitObjColType">相手のどの判定に当たったか</param>
		/// <param name="hitObjArrayNum">相手の何個目の判定に当たったか</param>
		virtual void Hit
		(
			const GameObject* const  object,
			const ShapeType3D& collisionType,
			const int arrayNum,
			const ShapeType3D& hitObjColType,
			const int hitObjArrayNum
		);



		//void CalcHitPhysics(GameObject* hitObject,const Vector3& hutPreVelocity,const CollisionType& collisionType);

		//virtual const void* GetPtr()const;



		//確実に初期値が決まっている変数(eraseManagerなど)を初期化する変数(初期化忘れ防止用)
		//これで初期化せずにヘッダで初期化する?
		//再追加したときに初期化したいからこのままでいい
		void FalsEraseManager();



#pragma region 加算
		/// <summary>
		/// 座標、モデルの座標、判定の座標に引数を加算します。
		/// </summary>
		/// <param name="vec"></param>
	    virtual void AddPosition(const Vector3& vec);
#pragma endregion


#pragma region セット

		/// <summary>
		/// GameObject共通の重力加速度をセットします。
		/// </summary>
		/// <param name="acc"></param>
		static void SetGravutationalAcceleration(const float acc) { gravutationalAcc = acc; };
			
		/// <summary>
		/// 座標をセットします。モデルと衝突確認に使うデータは、セット前の座標との差だけ移動します。
		/// </summary>
		/// <param name="pos"></param>
		virtual void SetPosition(const Vector3& pos);

		/// <summary>
		/// 角度をセットします。
		/// </summary>
		/// <param name="angle"></param>
		virtual void SetAngle(const Vector3& angle);

		virtual void SetScale(const Vector3& scale);

		/// <summary>
		/// 力をセットします。
		/// </summary>
		/// <param name="force"></param>
		void SetForce(const Vector3& force) { this->force = force; }
		
		/// <summary>
		/// 重さをセットします。
		/// </summary>
		/// <param name="mass"></param>
		void SetMass(const float mass) { this->mass = mass; }

		void SetAddColor(const Color& color);
		void SetSubColor(const Color& color);
		void SetMulColor(const Color& color);

		void TrueEraseManager() { eraseManager = true; }
#pragma endregion

#pragma region ゲット
		/// <summary>
		/// GameObject共通の重力加速度を取得します。
		/// </summary>
		/// <returns></returns>
		static float GetGravutationalAcceleration() { return gravutationalAcc; };

		/// <summary>
		/// 座標を取得します。
		/// </summary>
		/// <returns></returns>
		Vector3 GetPosition()const { return position; }

		/// <summary>
		/// 速度を取得します。
		/// </summary>
		/// <returns></returns>
		Vector3 GetVelocity()const { return velocity; }

		/// <summary>
		/// 加速度を取得します。
		/// </summary>
		/// <returns></returns>
		Vector3 GetAcceleration()const { return acceleration; }

		/// <summary>
		/// 加えられている力を取得します。
		/// </summary>
		/// <returns></returns>
		Vector3 GetForce()const { return force; }

		/// <summary>
		/// 重さを取得します。
		/// </summary>
		/// <returns></returns>
		float GetMass()const { return mass; }
		
		/// <summary>
		/// 落下中または投げ上げ中かどうかを取得します。
		/// </summary>
		/// <returns></returns>
		bool GetIsFall()const { return isFall; }
		
		short GetSortNumber() const { return sortNumber; }


		std::vector<std::string>GetTags()const { return tags; }

		/// <summary>
		/// ObjectManagerから削除するフラグを返します。
		/// </summary>
		/// <returns></returns>
		bool GetEraseManager()const { return eraseManager; }

#pragma region モデル関係
		const std::unordered_map<std::string, ModelObject>& GetRefModelObjects()const { return modelObjects; }
#pragma endregion

#pragma region 判定関係
		
#pragma endregion


#pragma endregion




#pragma region 判定用関数


		// 判定用関数
		CollisionDetectionFlag GetCollisionFlag() const { return collisionFlag; }
		std::vector<SphereData> GetSphereData() const { return sphereData; }
		std::vector<BoxData> GetBoxData() const { return boxData; }
		std::vector<Segment3DData> GetSegmentData() const { return segment3DData; }
		std::vector<PlaneData> GetPlaneData() const { return planeData; }
		std::vector<BoardData> GetBoardData()const { return boardData; }
		std::vector<CapsuleData>GetCapsuleData() const { return capsuleData; }
		std::vector<TriangleData>GetTriangleData() const { return triangleData; }

		// ここ参照取得じゃなくてSetにする?
		// そもそも持たせない。Hit関数で渡す
		void SetSphereCalcResult(const SphereCalcResult& result, const UINT index) { sphereData[index].SetCalcResult(result); }
		void SetBoxCalcResult(const BoxCalcResult& result, const UINT index) { boxData[index].SetCalcResult(result); }
		void SetSegmentCalcResult(const Segment3DCalcResult& result, const UINT index) { segment3DData[index].SetCalcResult(result); }
		void SetBoardCalcResult(const BoardCalcResult& result, const UINT index) { boardData[index].SetCalcResult(result); }
		void SetCapsuleCalcResult(const Segment3DCalcResult& result, const UINT index)
		{ capsuleData[index].GetRefSegment3DData().SetCalcResult(result); }
		void SetTriangleCalcResult(const TriangleCalcResult& result, const UINT index)
		{
			triangleData[index].SetCalcResult(result);
		}

		//Vector3& GetLineSegmentHitPosition(const int num);
		//Vector3& GetBoardHitPosition(const int num);
		//BoxHitDirection& GetSphereBoxHitDistance(const int num) { return sphereData[num].boxHitDistance; }
		//BoxHitDirection& GetBoxBoxHitDistance(const int num) { return boxData[num].boxHitDistance; }

		// ヒットした相手の当たり判定の情報を渡す関数。
		void SetHitSphereData(const SphereData& sphere) { hitSphereData = sphere; }
		void SetHitBoxData(const BoxData& box) { hitBoxData = box; }
		void SetHitBoardData(const BoardData& board) { hitBoardData = board; }
		void SetHitPlaneData(const PlaneData& plane) { hitPlaneData = plane; }
		void SetHitSegment3DData(const Segment3DData& segment) { hitSegment3DData = segment; }
		void SetHitCapsuleData(const CapsuleData& capsule) { hitCapsuleData = capsule; }
		void SetHitTriangleData(const TriangleData& tri) { hitTriangleData = tri; }

#ifdef _DEBUG
		static void CreateCollisionCheckModelPipelineState();

		//衝突確認用モデル生成、削除
		void CreateCollisionCheckModel();
		//衝突確認用モデルのデータセット
		void SetCollisionCheckModelData();
		//衝突確認用モデルの描画
		void DrawCollisionCheckModel();

#endif // _DEBUG



#pragma endregion

	};
}