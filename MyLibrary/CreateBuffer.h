#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXTex.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include"DirectXStruct.h"
#include"Color.h"

namespace MelLib
{
	//バッファーを作成するためのクラス
	class CreateBuffer final
	{
	private:
		//ここComPtrにするか確認する
		ID3D12Device* device;
		int windowWidth, windowHeight;

		CreateBuffer();
		~CreateBuffer();
	public:

		CreateBuffer(CreateBuffer& c) = delete;
		CreateBuffer& operator=(CreateBuffer& c) = delete;
		static CreateBuffer* GetInstance();

		void Initialize
		(
			ID3D12Device* device,
			const int& windowWidth,
			const int& windowHeight
		);

#pragma region 旧

		//ここvoidじゃなくてboolにする
		void CreateVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<Vertex> vertices, VertexBufferSet& set);
		bool CreatePMDVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<PMDVertex> vertices, PMDVertexBufferSet& set);

		bool CreateUserVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, void** vertexData, unsigned int vertexDataSize, unsigned int vertexSumDataSize, VertexBufferSet& set);

		void CreateIndexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<unsigned short> indices, IndexBufferSet& set);


		void CreateConstBufferSet(D3D12_HEAP_PROPERTIES cbheapprop, D3D12_RESOURCE_DESC cbresdesc, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, void** constData, ConstBufferSet& set, int num);//(numは、ヒープの要素数)
		void CreateTextureBufferSet(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, DirectX::TexMetadata metadata, const DirectX::Image* image, TextureBufferSet& set, int num);
		void CreateTextureBufferSet2(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, Color color, TextureBufferSet& set, int num);

		void CreateDepthBufferSet(D3D12_HEAP_PROPERTIES depthheapprop, D3D12_RESOURCE_DESC debthresdesc, D3D12_CLEAR_VALUE depthclearvalue, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, DepthBufferSet& set);

#pragma region ユーザー作成
		//void createVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<void*> vertices, VertexBufferSet& set);

		//void createUserConstBufferSet(D3D12_HEAP_PROPERTIES cbheapprop, D3D12_RESOURCE_DESC cbresdesc, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, void* constBuffData, UserConstBufferSet& set);

#pragma endregion

#pragma endregion


#pragma region 新

#pragma region バッファ作成


	/// <summary>
	/// 
	/// </summary>
	/// <param name="heapprop">プロップ</param>
	/// <param name="verticesSize">sizeofで計算した構造体のバイト数</param>
	/// <param name="verticesNum">頂点数</param>
	/// <param name="set">バッファーセット</param>
		void CreateVertexBuffer
		(
			const D3D12_HEAP_PROPERTIES& heapprop,
			const size_t& verticesSize,
			const size_t& verticesNum,
			VertexBufferSet& set
		);

		void CreateIndexBuffer
		(
			const D3D12_HEAP_PROPERTIES& heapprop,
			const std::vector<USHORT>& indices,
			IndexBufferSet& set
		);

		void CreateConstBuffer
		(
			const D3D12_HEAP_PROPERTIES& cbheapprop,
			const size_t& constStructDataSize,
			ID3D12Resource** constBuffer
		);

		void CreateTextureBuffer
		(
			const DirectX::TexMetadata& metadata,
			const DirectX::Image* image,
			ID3D12Resource** textureBuffer
		);

		void CreateTextureBufferAndView
		(
			const DirectX::TexMetadata& metadata,
			const DirectX::Image* image,
			const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle,
			ID3D12Resource** textureBuffer
		);

		void CreateSpriteTextureBufferAndView
		(const DirectX::TexMetadata& metadata,
			const DirectX::Image* image,
			const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle,
			ID3D12Resource** textureBuffer
		);

		void CreateOneColorTextureBuffer
		(
			const Color& color,
			ID3D12Resource** textureBuffer
		);

		void CreateOneColorTextureBufferAndView
		(
			const Color& color,
			const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle,
			ID3D12Resource** textureBuffer
		);

		void CreateTexture3DBuffer
		(
			const DirectX::TexMetadata& metadata,
			const std::vector <const DirectX::Image*>& image,
			ID3D12Resource** textureBuffer
		);

		void CreateDepthBuffer
		(
			const D3D12_HEAP_PROPERTIES& depthheapprop,
			const D3D12_RESOURCE_DESC& debthresdesc,
			const 	D3D12_CLEAR_VALUE& depthclearvalue,
			const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle,
			ID3D12Resource** depthBuffer
		);

		void CreateDescriptorHeap
		(
			ID3D12DescriptorHeap** heap,
			const int& arrayNum
		);
#pragma endregion

#pragma region ビュー
		void CreateConstBufferView
		(
			const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle,
			ID3D12Resource* pConstBuffer
		);

		void CreateShaderResourceView
		(
			const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle,
			ID3D12Resource* pTextureBuffer
		);
#pragma endregion



#pragma endregion

	};

}