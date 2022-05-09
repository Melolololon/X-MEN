#pragma once
#include"Vector.h"
#include"Sprite2D.h"
#include"CollisionDetectionData.h"

#include<unordered_map>

namespace MelLib 
{
	class GameObject2D
	{
	private:

		MelLib::Vector2 position;

		CircleData hitCircleData;
		RectData hitRectData;
		Segment2DData hitSegmentData;
		
		void MoveSpritePosition(const Vector2& vec);
		void MoveDataPosition(const Vector2& vec);

	protected:
		std::unordered_map<std::string, MelLib::Sprite2D>sprites;

		std::vector<MelLib::CircleData>circleDatas;
		std::vector<MelLib::RectData>rectDatas;
		std::vector<MelLib::Segment2DData>segmentDatas;

		CollisionDetectionFlag2D collisionFlags;
		std::vector<std::string>tags;
		bool eraseManager = false;
	public:
		//コンストラクタ
		GameObject2D();
		//デストラクタ
		virtual ~GameObject2D();

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
			const GameObject2D* const pObject,
			const ShapeType2D& dataType,
			const int num,
			const ShapeType2D& hitObjDataType,
			const int hitNum
		);



		void AddPosition(const MelLib::Vector2& vec);

#pragma region ゲット

		MelLib::Vector2 GetPosition()const { return position; }

		bool GetEraseManager()const { return eraseManager; }

		std::vector<std::string> GetTags()const { return tags; }
#pragma region 判定関係
		CollisionDetectionFlag2D GetCollisionFlag()const { return collisionFlags; }
		std::vector<CircleData>GetCircleData()const { return circleDatas; }
		std::vector<RectData>GetRectData()const { return rectDatas; }
		std::vector<Segment2DData>GetSegment2DData()const { return segmentDatas; }
#pragma endregion


#pragma endregion

#pragma region セット
		void FalseEraseManager();

		void SetPosition(const MelLib::Vector2& pos);

#pragma region 判定関係
		void SetHitCircleData(const CircleData& data) { hitCircleData = data; };
		void SetHitRectData(const RectData& data) { hitRectData = data; };
		void SetHitSegmentData(const Segment2DData& data) { hitSegmentData = data; };
#pragma endregion

#pragma endregion

	};
}
