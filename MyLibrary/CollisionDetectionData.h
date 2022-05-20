#pragma once
#include"Vector.h"
#include"Values.h"
namespace MelLib
{

	//CalcResultをCollisionResultにする?
	//CollisionDetectionData.hからPrimitiveTypeに変える?
	//衝突確認に使うデータだからこのままでいい?プリミティブじゃないデータ(線分)あるし
	//ShapeTypeの名前変える?

	//判定データに衝突確認関数持たせてもいい?

	//CalcResultをDataに持たせなくてもいいかも
	//Segment3Dなどのhit座標1つでいいかも

	//これヘッダーファイル分けたほうがいいかも

	// 名前HitCheckDataとかにしてもいいかも

	//衝突確認フラグ
	struct CollisionDetectionFlag
	{
		bool sphere = true;
		bool box = true;
		bool obb = true;
		bool ray = true;
		bool segment = true;
		bool plane = true;
		bool board = true;
		bool capsule = true;
		bool triangle = true;
	};

	struct CollisionDetectionFlag2D
	{
		bool circle = true;
		bool rect = true;
		bool segment = true;
	};

	//2Dの形状の列挙
	enum class ShapeType2D
	{
		RECT,
		CIRCLE,
		SEGMENT,
	};

	//3Dの形状の列挙
	enum class ShapeType3D
	{
		SPHERE,
		BOX,
		OBB,
		PLANE,
		TRIANGLE,
		BOARD,
		SEGMENT,
		RAY,
		CAPSULE,
	};


	enum class BoxHitDirection
	{
		BOX_HIT_DIRECTION_NO_HIT,
		BOX_HIT_DIRECTION_UP,
		BOX_HIT_DIRECTION_DOWN,
		BOX_HIT_DIRECTION_LEFT,
		BOX_HIT_DIRECTION_RIGHT,
		BOX_HIT_DIRECTION_FRONT,
		BOX_HIT_DIRECTION_BACK,
	};

	//データとリザルトまとめた構造体用意する?両方リサイズするの面倒だし

#pragma region 2D


#pragma region 四角形

//四角形
	class RectData
	{
	private:
		//四角形の左上
		Vector2 position;
		//辺の長さ
		Vector2 size;

	public:

		Vector2 GetPosition()const { return position; }
		Vector2 GetSize()const { return size; }

		void SetPosition(const Vector2& pos) { position = pos; }
		void SetSize(const Vector2& size) { this->size = size; }
	};
#pragma endregion

#pragma region 円

	//円の計算結果
	struct CircleCalcResult
	{
		Vector2 lineSegmentNearPosition;
	};

	//円
	class CircleData
	{
	private:
		Vector2 position;
		float r = 0.0f;

		CircleCalcResult result;
	public:
		Vector2 GetPosition()const { return position; }
		float GetRadius()const { return r; }
		CircleCalcResult GetCalcResult() { return result; }

		void SetPosition(const Vector2& pos) { position = pos; }
		void SetRadius(const float r) { this->r = r; }
		void SetCalcResult(const CircleCalcResult& result) { this->result = result; }
	};

#pragma endregion

#pragma region 扇形
	class CircularSectorData
	{
	private:
		CircleData circleData;

		//向き
		Vector2 direction;
		//角度(合計)
		float angle = 0.0f;

	public:
		CircleData GetCircleData()const { return circleData; }
		CircleData& GetRefCircleData() { return circleData; }
		Vector2 GetDirection()const { return direction; }
		float GetAngle()const { return angle; }

		void SetCircleData(const CircleData& data) { circleData = data; }
		void SetDirection(const Vector2 direction) { this->direction = direction; }
		void SetAngle(const float angle) { this->angle = angle; }
	};
#pragma endregion


#pragma region 線分2D
	enum class SegmentHitPlace
	{
		LS_HIT_POSITION_NOT_HIT,//衝突してない
		LS_HIT_POSITION_LE_START_END,//線の端(始点終点)
		LS_HIT_POSITION_LE_LINE,//線の端以外
	};

	struct Segment2DCalcResult
	{
		//当たってる座標
		Vector2 hitPos;
		//当たった場所
		SegmentHitPlace hitPlace;

		//最近点
		Vector2 nearPos;
	};

	class Segment2DData
	{
	private:
		//座標(始点終点)
		Value2<Vector2> position;

		Segment2DCalcResult result;
	public:

		Value2<Vector2> GetPosition()const { return position; }
		Segment2DCalcResult GetCalcResult()const { return result; }

		void SetPosition(const Value2<Vector2>& pos) { position = pos; }
		void SetCalcResult(const Segment2DCalcResult& result) { this->result = result; }
	};




#pragma endregion


#pragma endregion

#pragma region 3D

#pragma region 球


	class SphereCalcResult
	{
		//箱のどの部分にぶつかったかという情報
		BoxHitDirection boxHitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
		

		Vector3 boxHitSurfaceNormal;
		Vector3 obbHitSurfaceNormal;


	public:
		/// <summary>
		/// 当たったBOXのどこに当たったかを取得します。
		/// </summary>
		/// <returns></returns>
		BoxHitDirection GetBoxHitDirection()const { return boxHitDirection; }
		
		/// <summary>
		/// 当たったBOXの面の法線を取得します。
		/// </summary>
		/// <returns></returns>
		Vector3 GetBoxHitSurfaceNormal()const { return boxHitSurfaceNormal; }
		
		/// <summary>
		/// 当たったOBBの面の法線を取得します。
		/// </summary>
		/// <returns></returns>
		Vector3 GetOBBHitSurfaceNormal()const { return obbHitSurfaceNormal; }

		void SetBoxHitDirection(BoxHitDirection dir) { boxHitDirection = dir; }
		void SetBoxHitSurfaceNormal(const Vector3& normal) { boxHitSurfaceNormal = normal; }
		void SetOBBHitSurfaceNormal(const Vector3& normal) { obbHitSurfaceNormal = normal; }
	};

	//球
	class SphereData
	{
	private:
		//座標
		Vector3 position;

		//半径
		float r = 0.0f;

		SphereCalcResult result;

	public:
		Vector3 GetPosition()const { return position; }
		float GetRadius()const { return r; }
		SphereCalcResult GetCalcResult() const { return result; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetRadius(const float radius) { r = radius; }
		void SetCalcResult(const SphereCalcResult& result) { this->result = result; }
	};


#pragma endregion

#pragma region AABB
	struct BoxCalcResult
	{

		//相手がどこにぶつかったか
		BoxHitDirection boxHitDistance;
	};

	//箱
	class BoxData
	{
	private:
		Vector3 position;
		Vector3 size;
		BoxCalcResult result;

	public:
		Vector3 GetPosition()const { return position; }
		Vector3 GetSize()const { return size; }
		BoxCalcResult GetCalcResult() { return result; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetSize(const Vector3& size) { this->size = size; }
		void SetCalcResult(const BoxCalcResult& result) { this->result = result; }
	};



#pragma endregion

#pragma region OBB
	
	class OBBData
	{
	private:
		BoxData boxData;
		Vector3 angle;
	public:
		
		BoxData GetBoxData()const { return boxData; }
		BoxData& GetRefBoxData() { return boxData; }

		Vector3 GetPosition()const { return boxData.GetPosition(); }
		Vector3 GetSize()const { return boxData.GetSize(); }
		Vector3 GetAngle()const { return angle; }


		void SetPosition(const Vector3& pos) { boxData.SetPosition(pos); }
		void SetSize(const Vector3& size) { boxData.SetSize(size); }
		void SetAngle(const Vector3& angle) { this->angle = angle; }


	};

#pragma endregion

#pragma region 平面
	

	//平面
	class PlaneData
	{
	private:
		Vector3 position;
		Vector3 normal;
	public:
		Vector3 GetPosition()const { return position; }
		Vector3 GetNormal()const { return normal; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetNormal(const Vector3& normal) { this->normal = normal; }
	};
#pragma endregion

#pragma region 板

	struct BoardCalcResult
	{
		Vector3 lineSegment3DHitPos;
	};

	//板
	//板の判定。未回転時は、0,0,1の方向を向きます。
	class BoardData
	{
	private:
		Vector3 position;
		Vector2 size;
		Vector3 angle;
		BoardCalcResult result;

		Vector3 normal = Vector3(0, 0, -1);
		Vector3 leftDownPos;
		Vector3 leftUpPos;
		Vector3 rightUpPos;
		Vector3 rightDownPos;

	private:
		void CalcRotateDirPosition();

	public:
		Vector3 GetPosition()const { return position; }
		Vector2 GetSize()const { return size; }
		Vector3 GetAngle()const { return angle; }
		BoardCalcResult GetCalcResult()const { return result; }

		Vector3 GetNormal()const { return normal; }
		//左下、左上、右下、右上の順で格納されている
		Value4<Vector3>GetVertexPosition()const { return Value4<Vector3>(leftDownPos, leftUpPos, rightDownPos, rightUpPos); }


		void SetPosition(const Vector3& pos);
		void SetSize(const Vector2& size);
		void SetAngle(const Vector3& angle);
		void SetCalcResult(const BoardCalcResult& result) { this->result = result; }
	};


#pragma endregion

#pragma region 三角形
	struct TriangleCalcResult
	{
		Vector3 lineSegment3DHitPos;
	};

	class TriangleData
	{
	private:
		// 座標を3つまとめている
		Value3<Vector3>position;

		// 角度
		Vector3 angle;
		// 回転の座標
		Value3<Vector3>rotPos;

		//平行移動する量
		Vector3 translationVec;
		//平行移動後の座標
		Value3<Vector3>translationPos;

		// 法線
		Vector3 normal;

		// 現在未使用
		TriangleCalcResult result;
	private:
		void CalcNormal();

	public:
		Value3<Vector3> GetPosition()const { return position; }
		
		/// <summary>
		/// 開店後に平行移動する移動量を取得します。
		/// </summary>
		/// <returns></returns>
		Value3<Vector3> GetTranslationPosition()const { return translationPos; }
		Vector3 GetAngle()const { return angle; }
		Vector3 GetTransFormVector()const { return translationVec; }
		Vector3 GetNormal()const { return normal; }

		/// <summary>
		/// 三角形の中心座標を取得します。
		/// </summary>
		Vector3 GetCenter()const { return (position.v1 + position.v2 + position.v3) / 3; }

		/// <summary>
		/// 回転させた三角形の中心座標を取得します。
		/// </summary>
		Vector3 GetRotCenter()const { return (rotPos.v1 + rotPos.v2 + rotPos.v3) / 3; }

		/// <summary>
		/// 回転と平行移動した三角形の中心座標を取得します。
		/// </summary>
		Vector3 GetRotTranceFormCenter()const { return (translationPos.v1 + translationPos.v2 + translationPos.v3) / 3; }

		TriangleCalcResult GetCalcResult()const { return result; }

		/// <summary>
		/// 座標をセットします。頂点の順序で法線が変わります。
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Value3<Vector3>& pos);
		void SetAngle(const Vector3& angle);

		/// <summary>
		/// 回転後にどのくらい平行移動させるかを指定します。
		/// </summary>
		/// <param name="vec"></param>
		void SetTranslationPosition(const Vector3& vec);
		
		void SetCalcResult(const TriangleCalcResult& result) { this->result = result; }
	};
#pragma endregion


#pragma region 線分3D
	
	struct Segment3DCalcResult
	{
		Vector3 lineSegment3DHitPos;
		Vector3 boardHitPos;
		Vector3 triangleHitPos;
		Vector3 planeHitPos;
	};

	//線分
	class Segment3DData
	{
	private:
		Value2<Vector3> position;
		Vector3 angle = 0;
		//回転基準を決める変数(0.0fでv1基準、0.5fで中心、1.0fでv2基準)
		float rotatePoint = 0.5f;

		Vector3 axisAngle;


		Vector3 axisXVector = MelLib::Vector3(-1, 0, 0);
		Vector3 axisYVector = MelLib::Vector3(0, -1, 0);
		Vector3 axisZVector = MelLib::Vector3(0, 0, -1);

		//angleを適応させた座標
		Value2<Vector3> rotatePosition;

		Segment3DCalcResult result;



	private:
		//angle適応させた角度をrotatePositionに代入する関数
		void CalcRotatePosition();


	public:
		Value2<Vector3> GetPosition()const { return position; }
		Value2<Vector3> GetRotatePosition()const { return rotatePosition; }
		Vector3 GetAngle()const { return angle; }
		float GetRotatePoint()const { return rotatePoint; }
		Segment3DCalcResult GetCalcResult()const { return result; }


		void SetPosition(const Value2<Vector3>& pos);
		void SetAngle(const Vector3& angle);
		/// <summary>
		/// //回転基準を決める数値(0.0fでv1基準、0.5fで中心、1.0fでv2基準)をセットします。
		/// </summary>
		/// <param name="num">回転基準を決める数値(0.0fでv1基準、0.5fで中心、1.0fでv2基準)</param>
		void SetRotatePoint(const float num);

		/// <summary>
		/// 線分を回転させるのに使用するZXY軸自体の角度を設定します。
		/// </summary>
		/// <param name="angle"></param>
		void SetAxisAngle(const Vector3& angle);

		void SetCalcResult(const Segment3DCalcResult& result) { this->result = result; }

	};



#pragma endregion

#pragma region レイ

	struct RayCalcResult
	{
		Vector3 hitPosition;
	};

	//線
	class RayData
	{
	private:
		Vector3 position;
		Vector3 direction;

	public:
		Vector3 GetPosition()const { return position; }
		Vector3 GetDirection()const { return direction; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetDirection(const Vector3& dir) { direction = dir; }
	};
#pragma endregion


#pragma region カプセル


	//カプセル
	class CapsuleData
	{
		// 線分の参照取得しなくてもいじれるようにする
		// 継承はしなくていい
		// 別にカプセルを線分の配列に入れたいわけじゃないし
		// GameObjectみたいに配列にまとめるたりオーバーライドするつもりがないなら継承しないほうがよかったはず

	private:
		Segment3DData segmentData;
		float r = 0.0f;

	public:
		Segment3DData GetSegment3DData()const { return segmentData; }
		float GetRadius()const { return r; }

		Segment3DData& GetRefSegment3DData() { return segmentData; }

		void SetSegment3DData(const Segment3DData& data) { segmentData = data; }
		void SetRadius(const float radius) { r = radius; }

	};

#pragma endregion

#pragma region 錐台
	
	class FrustumData
	{
		//Boardだと台形表示できないといけない

	private:
		Vector3 position;

		Vector3 angle;
		//上下の角度
		Vector2 xyAngle = 1.0f;
		//手前
		float nearNum = 0.0001f;
		//奥
		float farNum = 1000.0f;

		//視錐台の平面(中にいるかを確認するためのもの)
		BoardData nearBoard;
		BoardData farBoard;
		BoardData leftBoard;
		BoardData rightBoard;
		BoardData upBoard;
		BoardData downBoard;

		Vector3 leftAngle;
		Vector3 rightAngle;
		Vector3 upAngle;
		Vector3 downAngle;

		/// <summary>
		/// xyAngleによる回転
		/// </summary>
		void XYRotate();

		/// <summary>
		/// angleによる回転
		/// </summary>
		void AngleRotate();

		void SetBoardPosition();

		/// <summary>
		/// 平面の距離の設定
		/// </summary>
		void SetPlaneDistance();

	public:
		FrustumData();

		Vector3 GetPosition()const { return position; }
		Vector3 GetAngle()const { return angle; }
		Vector2 GetXYAngle()const { return xyAngle; }
		Vector2 GetNear()const { return xyAngle; }
		Vector2 GetFar()const { return xyAngle; }
		std::vector<BoardData>GetBoardDatas()const;

		
		void SetPosition(const Vector3& pos);
		void SetAngle(const Vector3& angle);
		void SetXYAngle(const Vector2& angle);
		void SetNear(const float nearNum);
		void SetFar(const float farNum);

	
	};
#pragma endregion



#pragma region 扇形3D
	//class CircularSector3DData
	//{
	//private:
	//	SphereData sphereData;

	//	//向き
	//	Vector3 direction;
	//	//横の角度(合計)
	//	float widthAngle = 0.0f;
	//	//縦の角度(合計)
	//	float heightAngle = 0.0f;
	//public:
	//	SphereData GetSphereData()const { return sphereData; }
	//	SphereData& GetRefSphereData() { return sphereData; }
	//	Vector3 GetDirection()const { return direction; }
	//	float GetWidthAngle()const { return widthAngle; }
	//	float GetHeightAngle()const { return heightAngle; }

	//	void SetSphereData(const SphereData& data) { sphereData = data; }
	//	void SetDirection(const Vector3 direction) { this->direction = direction; }
	//	void SetHeightAngleAngle(const float angle) { this->heightAngle = angle; }
	//};
#pragma endregion



#pragma endregion

}