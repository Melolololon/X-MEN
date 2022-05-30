#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionDetectionData.h"
#include"Physics.h"

#include"ModelObject.h"

#ifdef _DEBUG
#include"PipelineState.h"
#include"Material.h"
#endif // _DEBUG



namespace MelLib
{
	class GameObject
	{
	private:

		// 開発者用
#ifdef _DEBUG
		//判定確認用モデルのパイプライン
		static PipelineState collisionCheckModelPipelineState;

		//判定確認用モデルのマテリアル
		static ADSAMaterial material;

		////判定確認用モデル

		//std::vector<ModelObject>sphereModelObjects;
		//std::vector<ModelObject>boxModelObjects;
		//std::vector<ModelObject>boardModelObjects;
		//std::array<std::vector<ModelObject>, 2>segmentModelObjects;

		////カプセル引き伸ばすと形崩れるから、球と円柱組み合わせて表示したほうがいいかも
		////判定のデータの変数は今のまま(角度消すと円柱を組み合わせるのが大変になる)
		//std::array<std::vector<ModelObject>, 2>capsuleModelObjects;

		// 新しいやつ
		std::unordered_map<std::string, std::vector<ModelObject>>sphereModelObjects;
		std::unordered_map<std::string, std::vector<ModelObject>>boxModelObjects;
		std::unordered_map<std::string, std::vector<ModelObject>>boardModelObjects;
		std::unordered_map<std::string, std::array<std::vector<ModelObject>, 2>>segmentModelObjects;
		std::unordered_map<std::string, std::array<std::vector<ModelObject>, 2>>capsuleModelObjects;



#endif // _DEBUG

		Vector3 position = 0;
		Vector3 angle = 0;
		Vector3 scale = 1;

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

		// 衝突確認時に計算された数値
		SphereCalcResult sphereCalcResult;
		BoxCalcResult boxCalcResult;
		Segment3DCalcResult segmentCalcResult;
		Segment3DCalcResult capsuleCalcResult;
		RayCalcResult rayCalcResult;
		BoardCalcResult boardCalcResult;
		TriangleCalcResult triangleCalcResult;
		

		// 各当たり判定の前フレームの座標
		// 当たり判定複数回確認するときに使用(補完用)
		std::unordered_map<std::string,std::vector<Vector3>>sphereDataPrePositions;
		std::unordered_map<std::string, std::vector<Vector3>>boxDataPrePositions;
		std::unordered_map<std::string, std::vector<Value2<Vector3>>>segment3DDataPrePositions;
		std::unordered_map<std::string, std::vector<Vector3>>rayDataPrePositions;
		std::unordered_map<std::string, std::vector<Vector3>>planeDataPrePositions;
		std::unordered_map<std::string, std::vector<Vector3>>boardDataPrePositions;
		std::unordered_map<std::string, std::vector<Value2<Vector3>>>capsuleDataPrePositions;
		std::unordered_map<std::string, std::vector<Value3<Vector3>>>triangleDataPrePositions;
		std::unordered_map<std::string, std::vector<Vector3>>obbDataPrePositions;

	protected:

#pragma region 判定データ
		CollisionDetectionFlag collisionFlag;

		/*std::vector<SphereData> sphereData;
		std::vector<BoxData> boxData;
		std::vector<Segment3DData> segment3DData;
		std::vector<RayData> layData;
		std::vector<PlaneData>planeData;
		std::vector<BoardData>boardData;
		std::vector<CapsuleData>capsuleData;
		std::vector<TriangleData>triangleData;*/

		// 判定新しいやつ
		// std::unordered_map<当たり判定の名前,当たり判定の配列>
		std::unordered_map<std::string, std::vector<SphereData>>sphereDatas;
		std::unordered_map<std::string, std::vector<BoxData>>boxDatas;
		std::unordered_map<std::string, std::vector<Segment3DData>>segment3DDatas;
		std::unordered_map<std::string, std::vector<RayData>>rayDatas;
		std::unordered_map<std::string, std::vector<PlaneData>>planeDatas;
		std::unordered_map<std::string, std::vector<BoardData>>boardDatas;
		std::unordered_map<std::string, std::vector<CapsuleData>>capsuleDatas;
		std::unordered_map<std::string, std::vector<TriangleData>>triangleDatas;
		std::unordered_map<std::string, std::vector<OBBData>>obbDatas;

		// 1だと、通常通り行う
		// 2だと通常のに加え、前フレームとの移動量の差の半分移動した場合の衝突確認も行う
		// 1フレームで行う衝突判定回数
		unsigned int sphereFrameHitCheckNum = 1;
		unsigned int boxFrameHitCheckNum = 1;
		unsigned int segment3DFrameHitCheckNum = 1;
		unsigned int rayFrameHitCheckNum = 1;
		unsigned int planeFrameHitCheckNum = 1;
		unsigned int boardFrameHitCheckNum = 1;
		unsigned int capsuleFrameHitCheckNum = 1;
		unsigned int triangleFrameHitCheckNum = 1;
		unsigned int obbFrameHitCheckNum = 1;

		// 衝突した時の補間した座標
		Vector3 lerpPosition;

		// 補間で動いた座標
		Vector3 lerpMovePosition;
#pragma endregion

		//継承したクラスを格納し、判定時に使う用
		std::vector<std::string>tags;

		std::unordered_map<std::string, ModelObject>modelObjects;

		//ソート用数値。ソート順を自分で調整するための変数
		// 2022/04/15 現在無効中
		short sortNumber = 0;

		//生死フラグ(これがtrueになると、オブジェクトマネージャーから除外される)
		bool eraseManager = false;

		// 判定表示するかどうか
		bool drawCollisionModel = true;

		protected:
			void SetAllCollisionFlag(const bool flag) 
			{

				collisionFlag.sphere = flag;
				collisionFlag.box = flag;
				collisionFlag.obb = flag;
				collisionFlag.ray = flag;
				collisionFlag.segment = flag;
				collisionFlag.plane = flag;
				collisionFlag.board = flag;
				collisionFlag.capsule = flag;
				collisionFlag.triangle = flag;
			}

			float collisionCheckDistance = 5.0f;
	private:
		/// <summary>
		/// 当たり判定数に応じてモデルの生成、削除を行う関数
		/// </summary>
		void CollisionCheckModelCreateOrDelete
		(
			const std::unordered_map<std::string, size_t>& datas,
			std::unordered_map<std::string, std::vector<ModelObject>>& modelObjects,
			const ShapeType3D type
		);



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


	protected:
		/// <summary>
		/// 全てのModelObjectを描画します。
		/// </summary>
		void AllDraw();
		
		// 衝突した判定を取得する関数
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

	public:

		//コンストラクタ
		GameObject();
		//デストラクタ
		virtual ~GameObject();

		//更新
		virtual void Update();
		//描画
		virtual void Draw();

		/*/// <summary>
		/// 当たった時の処理
		/// </summary>
		/// <param name="object">相手オブジェトのポインタ</param>
		/// <param name="collisionType">自分のどの判定に当たったか</param>
		/// <param name="arrayNum">自分の何個目の判定に当たったか</param>
		/// <param name="hitObjColType">相手のどの判定に当たったか</param>
		/// <param name="hitObjArrayNum">相手の何個目の判定に当たったか</param>*/

		/// <summary>
		/// 衝突した時に呼び出される判定
		/// </summary>
		/// <param name="object">オブジェクト</param>
		/// <param name="shapeType">自分のどの形状の判定と当たったか</param>
		/// <param name="shapeName">判定名</param>
		/// <param name="hitObjShapeType">相手の形状</param>
		/// <param name="hitShapeName">相手の判定名</param>
		virtual void Hit
		(
			/*const GameObject* const object,
			const ShapeType3D& collisionType,
			const int arrayNum,
			const ShapeType3D& hitObjColType,
			const int hitObjArrayNum*/
			const GameObject& object,
			const ShapeType3D shapeType,
			const std::string& shapeName,
			const ShapeType3D hitObjShapeType,
			const std::string& hitShapeName
		);



		//void CalcHitPhysics(GameObject* hitObject,const Vector3& hutPreVelocity,const CollisionType& collisionType);

		//virtual const void* GetPtr()const;



		//確実に初期値が決まっている変数(eraseManagerなど)を初期化する変数(初期化忘れ防止用)
		//これで初期化せずにヘッダで初期化する?
		//再追加したときに初期化したいからこのままでいい
		void FalseEraseManager();



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
		
		Vector3 GetAngle()const { return angle; }
		Vector3 GetScale()const { return scale; }

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

		/// <summary>
		/// シーンエディタからオブジェクト管理クラスに追加するときにオブジェクト管理クラスに追加するポインタを返す関数です。
		/// </summary>
		/// <returns>オブジェクト管理クラスに追加するshared_ptr</returns>
		virtual std::shared_ptr<GameObject> GetNewPtr();


#pragma endregion


#pragma region 判定用関数

		/// <summary>
		/// 前フレームの当たり判定の座標をセット
		/// Update呼び出し前に呼び出す
		/// </summary>
		void SetPreDataPositions();

		// 判定用関数
		CollisionDetectionFlag GetCollisionFlag() const { return collisionFlag; }
		//std::vector<SphereData> GetSphereData() const { return sphereData; }
		//std::vector<BoxData> GetBoxData() const { return boxData; }
		//std::vector<Segment3DData> GetSegmentData() const { return segment3DData; }
		//std::vector<PlaneData> GetPlaneData() const { return planeData; }
		//std::vector<BoardData> GetBoardData()const { return boardData; }
		//std::vector<CapsuleData>GetCapsuleData() const { return capsuleData; }
		//std::vector<TriangleData>GetTriangleData() const { return triangleData; }


		std::unordered_map<std::string, std::vector<SphereData>> GetSphereDatas() const { return sphereDatas; }
		std::unordered_map<std::string, std::vector<BoxData>> GetBoxDatas() const { return boxDatas; }
		std::unordered_map<std::string, std::vector<OBBData>> GetOBBDatas() const { return obbDatas; }
		std::unordered_map<std::string, std::vector<Segment3DData>> GetSegmentDatas() const { return segment3DDatas; }
		std::unordered_map<std::string, std::vector<PlaneData>> GetPlaneDatas() const { return planeDatas; }
		std::unordered_map<std::string, std::vector<BoardData>> GetBoardDatas()const { return boardDatas; }
		std::unordered_map<std::string, std::vector<CapsuleData>>GetCapsuleDatas() const { return capsuleDatas; }
		std::unordered_map<std::string, std::vector<TriangleData>>GetTriangleDatas() const { return triangleDatas; }



		// ここ参照取得じゃなくてSetにする?
		/*void SetSphereCalcResult(const SphereCalcResult& result, const UINT index) { sphereData[index].SetCalcResult(result); }
		void SetBoxCalcResult(const BoxCalcResult& result, const UINT index) { boxData[index].SetCalcResult(result); }
		void SetSegmentCalcResult(const Segment3DCalcResult& result, const UINT index) { segment3DData[index].SetCalcResult(result); }
		void SetBoardCalcResult(const BoardCalcResult& result, const UINT index) { boardData[index].SetCalcResult(result); }
		void SetCapsuleCalcResult(const Segment3DCalcResult& result, const UINT index)
		{
			capsuleData[index].GetRefSegment3DData().SetCalcResult(result);
		}
		void SetTriangleCalcResult(const TriangleCalcResult& result, const UINT index)
		{
			triangleData[index].SetCalcResult(result);
		}*/
		void SetSphereCalcResult(const SphereCalcResult& result) { sphereCalcResult = result; }
		void SetBoxCalcResult(const BoxCalcResult& result) { boxCalcResult = result;}
		void SetSegmentCalcResult(const Segment3DCalcResult& result) { segmentCalcResult = result;}
		void SetBoardCalcResult(const BoardCalcResult& result) { boardCalcResult = result;}
		void SetCapsuleCalcResult(const Segment3DCalcResult& result)
		{
			capsuleCalcResult = result;
		}
		void SetTriangleCalcResult(const TriangleCalcResult& result)
		{
			triangleCalcResult = result;
		}

		SphereCalcResult GetSphereCalcResult()const { return sphereCalcResult; }
		BoxCalcResult GetBoxCalcResult()const { return boxCalcResult; }
		Segment3DCalcResult GetSegmentCalcResult()const { return segmentCalcResult; }
		BoardCalcResult GetBoardCalcResult()const { return boardCalcResult; }
		Segment3DCalcResult GetCapsuleCalcResult()const { return capsuleCalcResult; }
		TriangleCalcResult GetTriangleCalcResult()const { return triangleCalcResult; }


		//Vector3& GetLineSegmentHitPosition(const int num);
		//Vector3& GetBoardHitPosition(const int num);
		//BoxHitDirection& GetSphereBoxHitDistance(const int num) { return sphereData[num].boxHitDistance; }
		//BoxHitDirection& GetBoxBoxHitDistance(const int num) { return boxData[num].boxHitDistance; }


		// 開発者用
		// ヒットした相手の当たり判定の情報を渡す関数。

		void SetHitSphereData(const SphereData& sphere) { hitSphereData = sphere; }
		void SetHitBoxData(const BoxData& box) { hitBoxData = box; }
		void SetHitBoardData(const BoardData& board) { hitBoardData = board; }
		void SetHitPlaneData(const PlaneData& plane) { hitPlaneData = plane; }
		void SetHitSegment3DData(const Segment3DData& segment) { hitSegment3DData = segment; }
		void SetHitCapsuleData(const CapsuleData& capsule) { hitCapsuleData = capsule; }
		void SetHitTriangleData(const TriangleData& tri) { hitTriangleData = tri; }

		unsigned int GetFrameHitCheckNumber(ShapeType3D type)const;

		void GetPreSpherePositions(std::unordered_map<std::string, std::vector<Vector3>>& refPos) { refPos = sphereDataPrePositions; }
		void GetPreBoxPositions(std::unordered_map<std::string, std::vector<Vector3>>& refPos) { refPos = boxDataPrePositions; }
		void GetPreRayPositions(std::unordered_map<std::string, std::vector<Vector3>>& refPos) { refPos = rayDataPrePositions; }
		void GetPrePlanePositions(std::unordered_map<std::string, std::vector<Vector3>>& refPos) { refPos = planeDataPrePositions; }
		void GetPreBoardPositions(std::unordered_map<std::string, std::vector<Vector3>>& refPos) { refPos = boardDataPrePositions; }
		void GetPreOBBPositions(std::unordered_map<std::string, std::vector<Vector3>>& refPos) { refPos = obbDataPrePositions; }
		void GetPreTrianglePositions(std::unordered_map<std::string, std::vector<Value3<Vector3>>>& refPos) { refPos = triangleDataPrePositions; }
		void GetPreSegment3DPositions(std::unordered_map<std::string, std::vector<Value2<Vector3>>>& refPos) { refPos = segment3DDataPrePositions; }
		void GetPreCapsule3DPositions(std::unordered_map<std::string, std::vector<Value2<Vector3>>>& refPos) { refPos = capsuleDataPrePositions; }


		void SetLerpPosition(const Vector3& pos) { lerpPosition = pos; }
		void SetLerpMovePosition(const Vector3& pos) { lerpMovePosition = pos; }

		/// <summary>
		/// 衝突確認を1フレームで複数回行ったときに補間して衝突した時の座標を返します。
		/// </summary>
		/// <returns></returns>
		Vector3 GetLerpPosition()const { return lerpPosition; }
		
		/// <summary>
		/// 衝突確認を1フレームで複数回行ったときに補間した時の移動量を返します。
		/// </summary>
		/// <returns></returns>
		Vector3 GetLerpMovePosition()const { return lerpMovePosition; }

		/// <summary>
		/// 衝突確認を1フレームで複数回行って補完した時に、衝突したオブジェクトと重ならないように押し出した座標を返します。
		/// </summary>
		/// <returns>重ならないように押し出した座標(GetLerpPosition() - GetLerpMovePosition())</returns>
		Vector3 GetLerpExtrudePosition()const { return lerpPosition - lerpMovePosition; }

		float GetCollisionCheckDistance()const { return collisionCheckDistance; }

		// 開発者用
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