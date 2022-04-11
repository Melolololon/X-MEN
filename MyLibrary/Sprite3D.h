#pragma once
#include"Sprite.h"
#include"PipelineState.h"
#include"RenderTarget.h"

namespace MelLib
{
	class Sprite3D :public Sprite
	{
	private:
		static std::unordered_map<std::string, std::unique_ptr<Sprite3D>> pSprite3D;
		static DirectX::XMMATRIX viewAndProjectionMatrix;
		static PipelineState defaultPipeline;

		static DirectX::XMFLOAT3 cameraPosition;
		static DirectX::XMFLOAT3 cameraTargetPosition;
		static DirectX::XMFLOAT3 cameraUpVector;

		bool billboardX = false;
		bool billboardY = false;
		bool billboardZ = false;


	private:
		void Billboard(DirectX::XMMATRIX& worldMat);
		void MatrixMap(const Camera* camera);

	protected:

	public:
		Sprite3D();
		Sprite3D(const Color& color);
		Sprite3D(Texture* pTexture);
		Sprite3D(const Sprite3D& sprite);
		~Sprite3D() {}

		static void Create(const Color& color, const std::string& name);
		static void Create(Texture* pTexture, const std::string& name);
		static void Delete(const std::string& name);
		static Sprite3D* Get(const std::string& name) { return pSprite3D[name].get(); }

		void Create()override;
		void Create(const Color& color)override;
		void Create(Texture* pTexture)override;


		//レンダーターゲットでDrawを使うため、仮想関数にしてる
		virtual void Draw(const std::string& rtName = RenderTarget::GetMainRenderTargetNama())override;

#pragma region 開発者用関数


		static bool Initialize();

		static void SetViewAndProjectionMatrix(const DirectX::XMMATRIX& cameraMat) { viewAndProjectionMatrix = cameraMat; }
		static void SetCameraPosTargetUpVector
		(
			const DirectX::XMFLOAT3& pos,
			const DirectX::XMFLOAT3& target,
			const DirectX::XMFLOAT3& up
		)
		{
			cameraPosition = pos;
			cameraTargetPosition = target;
			cameraUpVector = up;
		}
#pragma endregion

#pragma region セット


		void SetColor(const Color& color) { this->color = color; }
		void SetTexture(Texture* pTexture) { this->pTexture = pTexture; }

#pragma region 操作
		void SetPosition(const Vector3& pos) { constData.position = pos.ToXMFLOAT3(); }
		void SetAngle(const Vector3& angle) { constData.angle = angle.ToXMFLOAT3(); }
		void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
#pragma endregion
		void SetBillboardFlag(bool flagX, bool flagY, bool flagZ);


#pragma endregion

#pragma region ゲット
		Vector3 GetPosition()const { return constData.position; }
		Vector3 GetAngle()const { return constData.angle; }
		Vector2 GetScale()const { return constData.scale; }
#pragma endregion


		static PipelineState GetDefaultPipeline() { return defaultPipeline; }


	};
}
