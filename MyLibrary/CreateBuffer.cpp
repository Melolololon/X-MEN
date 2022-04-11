#include "CreateBuffer.h"
#include<d3dx12.h>

using namespace MelLib;

CreateBuffer::CreateBuffer()
{
}
CreateBuffer::~CreateBuffer() {}


CreateBuffer* CreateBuffer::GetInstance()
{
	static CreateBuffer c;
	return &c;
}


void CreateBuffer::Initialize
(
	ID3D12Device* device,
	const int& windowWidth,
	const int& windowHeight
)
{
	this->device = device;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}


#pragma region 旧



#pragma region 頂点

void CreateBuffer::CreateVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<Vertex> vertices, VertexBufferSet& set)
{
	device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.vertexBuffer)
	);



	set.vertexBuffer->Map(0, nullptr, (void**)&set.vertexMap);

	for (int j = 0; j < (int)vertices.size(); j++)
	{
		set.vertexMap[j] = vertices[j];
	}

	set.vertexBuffer->Unmap(0, nullptr);

	set.vertexBufferView.BufferLocation = set.vertexBuffer->GetGPUVirtualAddress();
	set.vertexBufferView.SizeInBytes = sizeof(Vertex) * vertices.size();
	set.vertexBufferView.StrideInBytes = sizeof(Vertex);


}


bool CreateBuffer::CreatePMDVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<PMDVertex> vertices, PMDVertexBufferSet& set)
{
	auto result = device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.vertexBuffer)
	);

	if (result != S_OK)return false;

	set.vertexBuffer->Map(0, nullptr, (void**)&set.vertexMap);
	for (int j = 0; j < (int)vertices.size(); j++)
	{
		set.vertexMap[j] = vertices[j];
	}
	set.vertexBuffer->Unmap(0, nullptr);

	set.vertexBufferView.BufferLocation = set.vertexBuffer->GetGPUVirtualAddress();
	set.vertexBufferView.SizeInBytes = sizeof(PMDVertex) * vertices.size();
	set.vertexBufferView.StrideInBytes = sizeof(PMDVertex);

	return true;
}


bool CreateBuffer::CreateUserVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, void** vertexData, unsigned int vertexDataSize, unsigned int vertexSumDataSize, VertexBufferSet& set)
{

	auto result = device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.vertexBuffer)
	);

	if (result != S_OK)return false;

	result = set.vertexBuffer->Map(0, nullptr, vertexData);


	//set.vertexBuffer->Unmap(0, nullptr);

	set.vertexBufferView.BufferLocation = set.vertexBuffer->GetGPUVirtualAddress();
	set.vertexBufferView.SizeInBytes = vertexSumDataSize;
	set.vertexBufferView.StrideInBytes = vertexDataSize;

	return true;
}

#pragma endregion

void CreateBuffer::CreateIndexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<unsigned short> indices, IndexBufferSet& set)
{
	device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.indexBuffer)
	);

	set.indexBuffer->Map(0, nullptr, (void**)&set.indexMap);
	for (int j = 0; j < (int)indices.size(); j++)
	{
		set.indexMap[j] = indices[j];
	}
	set.indexBuffer->Unmap(0, nullptr);

	set.indexBufferView.BufferLocation = set.indexBuffer->GetGPUVirtualAddress();
	set.indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	set.indexBufferView.SizeInBytes = sizeof(unsigned short) * indices.size();
}


void CreateBuffer::CreateConstBufferSet(D3D12_HEAP_PROPERTIES cbheapprop, D3D12_RESOURCE_DESC cbresdesc, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, void** constData, ConstBufferSet& set, int num)
{

	auto result = device->CreateCommittedResource(
		&cbheapprop,
		D3D12_HEAP_FLAG_NONE,
		&cbresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.constBuffer[num]));

	result = set.constBuffer[num]->Map(0, nullptr, constData);

	//これを2Dか3Dかで入れるものを変える
	//set.constMaps->mat = constBufferData.mat;

	/*set.constMaps->mat.r[0].m128_f32[0] = 2.0f / windowWidth;
	set.constMaps->mat.r[1].m128_f32[1] = -2.0f / windowHeight;
	set.constMaps->mat.r[3].m128_f32[0] = -1.0f;
	set.constMaps->mat.r[3].m128_f32[1] = 1.0f;*/

	//set.constBuffer->Unmap(0, nullptr);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;

	cbvDesc.BufferLocation = set.constBuffer[num]->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)set.constBuffer[num]->GetDesc().Width;
	device->CreateConstantBufferView(&cbvDesc, heapHandle);
	
}

void CreateBuffer::CreateTextureBufferSet(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, DirectX::TexMetadata metadata, const DirectX::Image* image, TextureBufferSet& set, int num)
{
	D3D12_HEAP_PROPERTIES texHeapprop{};
	D3D12_RESOURCE_DESC texResDesc{};

	texHeapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapprop.CreationNodeMask = 0;
	texHeapprop.VisibleNodeMask = 0;

	texResDesc.Format = metadata.format;
	texResDesc.Width = metadata.width;
	texResDesc.Height = (UINT)metadata.height;
	texResDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 1;
	texResDesc.MipLevels = (UINT16)metadata.mipLevels;
	texResDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	device->CreateCommittedResource(
		&texHeapprop,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.textureBuffer[num]));

	auto result = set.textureBuffer[num]->WriteToSubresource
	(
		0,
		nullptr,
		image->pixels,
		(UINT)image->rowPitch,
		(UINT)image->slicePitch
	);

	//ここがおかしい?
	//Formatがおかしかった
	//またここおかしくなった
	//ハンドルがダメ?ポインタがダメ?
	//ハンドルおかしくないマジわけわからん
	//TextureBufferSetだと読み込めないが、std;;vector<TextureBufferSet>だと読み込める
	//なぜかベクターだとエラーでない。ちなみに配列は無理だった。
	//D3D12_RESOURCE_STATE_GENERIC_READを書かないといけないのにD3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCEを書いていた
	//Desc書く部分にViewを書いてたからエラー
	//Viewだと思ってたのはDescだった
	//Format違いと構造体のDescを使っていたのが問題だった
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView
	(
		set.textureBuffer[num].Get(),
		&srvDesc,
		heapHandle
	);

}

void CreateBuffer::CreateTextureBufferSet2(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, Color color, TextureBufferSet& set, int num)
{
	D3D12_HEAP_PROPERTIES texHeapprop{};
	D3D12_RESOURCE_DESC texResDesc{};

	texHeapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapprop.CreationNodeMask = 0;
	texHeapprop.VisibleNodeMask = 0;

	texResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texResDesc.Width = 20;
	texResDesc.Height = 20;
	texResDesc.DepthOrArraySize = 1;
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 1;
	texResDesc.MipLevels = 1;
	texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto result = device->CreateCommittedResource(
		&texHeapprop,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.textureBuffer[num]));



	std::vector<Color>c(20 * 20);
	for (auto& rgba : c)
	{
		rgba.r = color.r;
		rgba.g = color.g;
		rgba.b = color.b;
		rgba.a = color.a;

	}

	result = set.textureBuffer[num]->WriteToSubresource
	(
		0,
		nullptr,
		c.data(),
		sizeof(Color) * 20,
		sizeof(Color) * c.size()
	);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView
	(
		set.textureBuffer[num].Get(),
		&srvDesc,
		heapHandle
	);
}


void CreateBuffer::CreateDepthBufferSet(D3D12_HEAP_PROPERTIES depthheapprop, D3D12_RESOURCE_DESC depthresdesc, D3D12_CLEAR_VALUE depthclearvalue, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, DepthBufferSet& set)
{


	device->CreateCommittedResource
	(
		&depthheapprop,
		D3D12_HEAP_FLAG_NONE,
		&depthresdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthclearvalue,
		IID_PPV_ARGS(&set.depthBuffer)
	);

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView
	(
		set.depthBuffer.Get(),
		&dsvDesc,
		heapHandle
	);
}


#pragma endregion



#pragma region 新

#pragma region バッファ

void CreateBuffer::CreateVertexBuffer
(
	const D3D12_HEAP_PROPERTIES& heapprop,
	const size_t& verticesSize,
	const size_t& verticesNum,
	VertexBufferSet& set
)
{

	D3D12_RESOURCE_DESC resdesc;

	resdesc = CD3DX12_RESOURCE_DESC::Buffer(verticesSize * verticesNum);

	device->CreateCommittedResource
	(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.vertexBuffer)
	);

	set.vertexBufferView.BufferLocation = set.vertexBuffer->GetGPUVirtualAddress();
	set.vertexBufferView.SizeInBytes = verticesSize * verticesNum;
	set.vertexBufferView.StrideInBytes = verticesSize;

}


void CreateBuffer::CreateIndexBuffer
(
	const D3D12_HEAP_PROPERTIES& heapprop,
	const std::vector<USHORT>& indices,
	IndexBufferSet& set
)
{
	auto indexNum = indices.size();
	auto indexSize = sizeof(USHORT) * indexNum;

	D3D12_RESOURCE_DESC resdesc = CD3DX12_RESOURCE_DESC::Buffer(indexSize);

	device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.indexBuffer)
	);

	set.indexBuffer->Map(0, nullptr, (void**)&set.indexMap);
	for (int j = 0; j < indexNum; j++)
		set.indexMap[j] = indices[j];
	set.indexBuffer->Unmap(0, nullptr);

	set.indexBufferView.BufferLocation = set.indexBuffer->GetGPUVirtualAddress();
	set.indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	set.indexBufferView.SizeInBytes = indexSize;

}

void CreateBuffer::CreateConstBuffer
(
	const D3D12_HEAP_PROPERTIES& cbheapprop,
	const size_t& constStructDataSize,
	ID3D12Resource** constBuffer
)
{
	D3D12_RESOURCE_DESC resDesc = 
		CD3DX12_RESOURCE_DESC::Buffer((constStructDataSize + 0xff) & ~0xff);
		
	auto result = 
		device->CreateCommittedResource
		(
			&cbheapprop,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(constBuffer)
		);

}

void MelLib::CreateBuffer::CreateTextureBuffer(const DirectX::TexMetadata& metadata, const DirectX::Image* image, ID3D12Resource** textureBuffer)
{
	D3D12_HEAP_PROPERTIES texHeapprop{};
	D3D12_RESOURCE_DESC texResDesc{};

	texHeapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapprop.CreationNodeMask = 0;
	texHeapprop.VisibleNodeMask = 0;

	texResDesc.Format = metadata.format;
	texResDesc.Width = static_cast<UINT>(metadata.width);
	texResDesc.Height = static_cast<UINT>(metadata.height);
	texResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);//深度(w方向の数、3Dテクスチャ用)
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 1;
	texResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
	//texResDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	texResDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	device->CreateCommittedResource
	(
		&texHeapprop,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureBuffer)
	);

	ID3D12Resource* r = *textureBuffer;
	auto result = r->WriteToSubresource
	(
		0,
		nullptr,
		image->pixels,
		(UINT)image->rowPitch,
		(UINT)image->slicePitch
	);

}


void CreateBuffer::CreateTextureBufferAndView
(
	const DirectX::TexMetadata& metadata,
	const DirectX::Image* image,
	const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle,
	ID3D12Resource** textureBuffer
)
{
	CreateTextureBuffer(metadata, image, textureBuffer);

	CreateShaderResourceView
	(
		heapHandle,
		*textureBuffer
	);
}

void MelLib::CreateBuffer::CreateSpriteTextureBufferAndView(const DirectX::TexMetadata& metadata, const DirectX::Image* image, const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle, ID3D12Resource** textureBuffer)
{
	D3D12_HEAP_PROPERTIES texHeapprop{};
	D3D12_RESOURCE_DESC texResDesc{};

	texHeapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapprop.CreationNodeMask = 0;
	texHeapprop.VisibleNodeMask = 0;

	texResDesc.Format = metadata.format;
	texResDesc.Width = static_cast<UINT>(metadata.width);
	texResDesc.Height = static_cast<UINT>(metadata.height);
	texResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);//深度(w方向の数、3Dテクスチャ用)
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 1;
	texResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
	texResDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	device->CreateCommittedResource
	(
		&texHeapprop,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureBuffer)
	);

	ID3D12Resource* r = *textureBuffer;
	auto result = r->WriteToSubresource
	(
		0,
		nullptr,
		image->pixels,
		(UINT)image->rowPitch,
		(UINT)image->slicePitch
	);

	CreateShaderResourceView
	(
		heapHandle,
		*textureBuffer
	);
}

void MelLib::CreateBuffer::CreateOneColorTextureBuffer(const Color& color, ID3D12Resource** textureBuffer)
{
	D3D12_HEAP_PROPERTIES texHeapprop{};
	D3D12_RESOURCE_DESC texResDesc{};

	texHeapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapprop.CreationNodeMask = 0;
	texHeapprop.VisibleNodeMask = 0;

	texResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texResDesc.Width = 1;
	texResDesc.Height = 1;
	texResDesc.DepthOrArraySize = 1;
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 1;
	texResDesc.MipLevels = 1;
	texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto result = device->CreateCommittedResource
	(
		&texHeapprop,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureBuffer)
	);

	ID3D12Resource* r = *textureBuffer;


	//テクスチャサイズ1*1
	result = r->WriteToSubresource
	(
		0,
		nullptr,
		&color,
		sizeof(Color),//1列(横)
		sizeof(Color)//全体サイズ
	);
}

void CreateBuffer::CreateOneColorTextureBufferAndView
(
	const Color& color,
	const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle,
	ID3D12Resource** textureBuffer
)
{
	D3D12_HEAP_PROPERTIES texHeapprop{};
	D3D12_RESOURCE_DESC texResDesc{};

	texHeapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapprop.CreationNodeMask = 0;
	texHeapprop.VisibleNodeMask = 0;

	texResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texResDesc.Width = 1;
	texResDesc.Height = 1;
	texResDesc.DepthOrArraySize = 1;
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 1;
	texResDesc.MipLevels = 1;
	texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto result = device->CreateCommittedResource
	(
		&texHeapprop,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureBuffer)
	);

	ID3D12Resource* r = *textureBuffer;
	

	//テクスチャサイズ1*1
	result = r->WriteToSubresource
	(
		0,
		nullptr,
		&color,
		sizeof(Color),//1列(横)
		sizeof(Color)//全体サイズ
	);

	CreateShaderResourceView
	(
		heapHandle,
		*textureBuffer
	);
}

void MelLib::CreateBuffer::CreateTexture3DBuffer(const DirectX::TexMetadata& metadata, const std::vector<const DirectX::Image*>& image, ID3D12Resource** textureBuffer)
{
	D3D12_HEAP_PROPERTIES texHeapprop{};
	D3D12_RESOURCE_DESC texResDesc{};

	int depth = image.size();

	texHeapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapprop.CreationNodeMask = 0;
	texHeapprop.VisibleNodeMask = 0;

	texResDesc.Format = metadata.format;
	texResDesc.Width = static_cast<UINT>(metadata.width);
	texResDesc.Height = static_cast<UINT>(metadata.height);
	texResDesc.DepthOrArraySize = depth;//深度(w方向の数、3Dテクスチャ用)
	//texResDesc.DepthOrArraySize = 12;//深度(w方向の数、3Dテクスチャ用)
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 1;
	texResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
	texResDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	device->CreateCommittedResource
	(
		&texHeapprop,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureBuffer)
	);

	//std::vector<uint8_t*>pixels3D(depth);
	//for (int i = 0; i < depth; i++)
	//{
	//	pixels3D[i] = image[i]->pixels;
	//}

	////試し
	////1要素1ピクセル
	//std::vector<Color>pix(metadata.width * metadata.height * depth);
	//
	//for(int z = 0; z < depth;z++)
	//{
	//	int pad = 0;
	//	for (int y = 0; y < metadata.height; y++)
	//	{
	//		for (int x = 0; x < metadata.width; x++)
	//		{

	//			UINT index = x + (y * metadata.width) + (z * metadata.width * metadata.height);
	//			pix[index].r = image[z]->pixels[x + (y * metadata.width) + pad];
	//			pix[index].g = image[z]->pixels[1 + x + (y * metadata.width) + pad];
	//			pix[index].b = image[z]->pixels[2 + x + (y * metadata.width) + pad];
	//			pix[index].a = image[z]->pixels[3 + x + (y * metadata.width) + pad];
	//			pad++;
	//		}
	//	}
	//}

	////試し2
	std::vector<unsigned char>pixel3D((UINT)image[0]->slicePitch * depth);
	for (int z = 0; z < depth; z++)
	{
		for (int s = 0; s < (UINT)image[0]->slicePitch; s++)
		{
			pixel3D[s + (z * (UINT)image[0]->slicePitch)] = image[z]->pixels[s];
		}
	}


	//2で生成できたから数値指定ミスってる?
	//pixels3Dのデータはちゃんと取得できた
	//テクスチャの読み込み枚数15にしたら読み込めたから枚数オーバー?
	//送ったテクスチャバグってる。1枚でもバグる
	//pixels3D[0]で転送できた。data()が悪い?
	//Imageのpixelってもしかして結構余分なデータ混ざってる?
	ID3D12Resource* r = *textureBuffer;
	auto result = r->WriteToSubresource
	(
		0,
		nullptr,
		pixel3D.data(),
		(UINT)image[0]->rowPitch,//Xの数
		(UINT)image[0]->slicePitch //縦*横(テクスチャ1枚のサイズ)
	);

	/*CreateShaderResourceView
	(
		heapHandle,
		*textureBuffer
	);*/
}

void CreateBuffer::CreateDepthBuffer
(
	const D3D12_HEAP_PROPERTIES& depthheapprop,
	const D3D12_RESOURCE_DESC& debthresdesc,
	const 	D3D12_CLEAR_VALUE& depthclearvalue,
	const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle,
	ID3D12Resource** depthBuffer
)
{
	device->CreateCommittedResource
	(
		&depthheapprop,
		D3D12_HEAP_FLAG_NONE,
		&debthresdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthclearvalue,
		IID_PPV_ARGS(depthBuffer)
	);

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView
	(
		*depthBuffer,
		&dsvDesc,
		heapHandle
	);
}

void CreateBuffer::CreateDescriptorHeap
(
	ID3D12DescriptorHeap** heap,
	const int& arrayNum
)
{
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};

	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = arrayNum;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	auto result = device->CreateDescriptorHeap
	(
		&descHeapDesc,
		IID_PPV_ARGS(heap)
	);
}



#pragma endregion


#pragma region ビュー

void CreateBuffer::CreateConstBufferView
(
	const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle,
	ID3D12Resource* pConstBuffer
)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = pConstBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = pConstBuffer->GetDesc().Width;
	device->CreateConstantBufferView
	(
		&cbvDesc, 
		heapHandle
	);
}

void CreateBuffer::CreateShaderResourceView
(
	const D3D12_CPU_DESCRIPTOR_HANDLE& heapHandle,
	ID3D12Resource* pTextureBuffer
)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = pTextureBuffer->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	switch (pTextureBuffer->GetDesc().Dimension)
	{
	case D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D:

		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		break;

	case D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE3D:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
		srvDesc.Texture3D.MipLevels = 1;
		break;
	}
	

	device->CreateShaderResourceView
	(
		pTextureBuffer,
		&srvDesc,
		heapHandle
	);
}

#pragma endregion


#pragma endregion

