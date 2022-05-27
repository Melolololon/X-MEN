#pragma once
#include"Sprite.h"
#include"ModelObject.h"

namespace MelLib 
{
	enum class DrawType
	{
		NONE,
		MODEL,
		SPRITE,
	};

	class DrawManager final
	{
	private:
		

		DrawManager() {}
		~DrawManager() {}

	private:
	
		DrawType preDrawType = DrawType::NONE;
		DrawType currentDrawType = DrawType::NONE;

		ModelData* preModelData = nullptr;
		ModelData* currentModelData = nullptr;

		const Texture* preTexture = nullptr;

		Material* preMaterial = nullptr;

		ID3D12Resource* preVertexBuffer = nullptr;
		ID3D12Resource* preIndexBuffer = nullptr;

		std::vector<ModelObject*>pModelObjects;
		std::vector<Sprite*> pSpriteDatas;

		static ID3D12Device* device;
		static std::vector<ID3D12GraphicsCommandList*>cmdLists;
		static ComPtr<ID3D12RootSignature>rootSignature;

	
	public:

		DrawManager(DrawManager& d) = delete;
		DrawManager& operator=(DrawManager& d) = delete;
		static DrawManager* GetInstance();


		void SetDraw(Sprite* sprite);
		void SetDraw(ModelObject* model);

		void Draw();

		void SetDrawData(ModelObject& model) 
		{
			preModelData = model.GetPModelData();
			preDrawType = DrawType::MODEL;
			preMaterial = model.GetPMaterial();

		}

		void SetView(ModelObject& model, const unsigned int index)
		{
			preVertexBuffer = model.GetPModelData()->GetVertexBufferSet()[index].vertexBuffer.Get();
			preIndexBuffer = model.GetPModelData()->GetIndexBufferSet()[index].indexBuffer.Get();
		}

		void SetDrawData(Sprite& sprite) 
		{
			preTexture = sprite.GetTexture();
			preDrawType = DrawType::SPRITE;
		}

		ModelData* GetPreModelData()const { return preModelData; }
		DrawType GetPreDrawType()const { return preDrawType; }
		const Texture* GetPreTexture()const { return preTexture; }
		Material* GetMaterial()const { return preMaterial; }

		ID3D12Resource* GetPreVertexBuffer()const { return preVertexBuffer; }
		ID3D12Resource* GetPreIndexBuffer()const { return preIndexBuffer; }

		void Reset() 
		{
			preDrawType = DrawType::NONE;

			preModelData = nullptr;
			preMaterial = nullptr;

			preTexture = nullptr;

			preVertexBuffer = nullptr;
			preIndexBuffer = nullptr;
		}
	};

}