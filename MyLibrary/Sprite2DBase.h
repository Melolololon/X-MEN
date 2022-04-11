#pragma once
#include"Sprite.h"
#include"PipelineState.h"

namespace MelLib
{
	//レンダーターゲットを分離したから消す
	//スプライト2D基底クラス
	class Sprite2DBase :public Sprite
	{
		
	private:

		//2Dのカメラは固定だから、行列生成関数このクラスに持たせてもいいかも
		static DirectX::XMMATRIX cameraMatrix;

		DirectX::XMFLOAT2 scalingPoint = DirectX::XMFLOAT2(0,0);
		DirectX::XMFLOAT2 rotationPoint = DirectX::XMFLOAT2(0,0);
	protected:


		static PipelineState defaultPipeline;


		void MatrixMap(Texture* texture);

		//void Create(const Color& color)override;
		//void Create(Texture* pTexture)override;

		//上の関数をSprite2Dに移動
		//新しい生成関数(Sprite2D、RTに使える関数)
		void SpriteInitialize();
	public:
		Sprite2DBase() {}
		//Sprite2DBase(const Color& color);
		//Sprite2DBase(Texture* pTexture);
		virtual ~Sprite2DBase() {}


		static bool Initialize(const int winWidth, const int winHeight);


		//void Draw(const std::string& rtName = "")override;



#pragma region 操作
		void SetPosition(const Vector2& pos) { constData.position = { pos.x,pos.y,0 }; }

		void SetAngle(const float& angle) { constData.angle = { 0,0,angle }; }

		/// <summary>
		/// 回転するときの基準座標を設定します。
		/// </summary>
		/// <param name="point">回転基準(0,0で左上を基準に回転)</param>
		void SetRotationPoint(const Vector2& point) { rotationPoint = point.ToXMFLOAT2(); }

		void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
		
		/// <summary>
		/// 拡縮するときの基準座標を設定します。
		/// </summary>
		/// <param name="point">拡縮基準(0,0で左上を基準に拡縮)</param>
		void SetScalingPoint(const Vector2& point) { scalingPoint = point.ToXMFLOAT2(); }
#pragma endregion

#pragma region ゲット
		Vector2 GetPosition()const { return Vector2(constData.position.x, constData.position.y); }

		float GetAngle()const { return constData.angle.z; }
		Vector2 GetScale()const { return Vector2(constData.scale.x, constData.scale.y); }
#pragma endregion



		static PipelineState GetDefaultPipeline() { return defaultPipeline; }
	};
}
