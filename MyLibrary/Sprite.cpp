#include "Sprite.h"
#include"CreateBuffer.h"
#include"PipelineState.h"
#include"DrawManager.h"

using namespace MelLib;

ID3D12Device* Sprite::device;
ID3D12GraphicsCommandList* Sprite::cmdList;
ComPtr<ID3D12RootSignature>Sprite::rootSignature;

std::vector<ComPtr<ID3D12Resource>> Sprite::textureBuffer;
ComPtr<ID3D12DescriptorHeap> Sprite::textureHeap;

Sprite::Sprite(){}

Sprite::~Sprite(){}

MelLib::Sprite::Sprite(const Sprite& sprite)
{
	drawMode = sprite.drawMode;
	pipeline = sprite.pipeline;
	vertices = sprite.vertices;
	drawLeftUpPosition = sprite.drawLeftUpPosition;
	drawRightDownPosition = sprite.drawRightDownPosition;
	constData = sprite.constData;
	color = sprite.color;
	pTexture = sprite.pTexture;
}

void Sprite::Draw(const std::string& rtName)
{
}

void MelLib::Sprite::SetDrawMode(const SpriteDrawMode mode)
{
	drawMode = mode;

	SpriteConstBufferData* constBufferData;
	constBuffer->Map(0, nullptr, (void**)&constBufferData);

	if (drawMode == SpriteDrawMode::DRAW_COLOR)
	{
		constBufferData->color = DirectX::XMFLOAT4
		(
			static_cast<float>(color.r) / 255.0f,
			static_cast<float>(color.g) / 255.0f,
			static_cast<float>(color.b) / 255.0f,
			static_cast<float>(color.a) / 255.0f
		);
	}
	else
	{
		constBufferData->color = DirectX::XMFLOAT4
		(
			0,
			0,
			0,
			0
		);
	}
	constBuffer->Unmap(0, nullptr);


}


void Sprite::InitializeVertices()
{
	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);
	vertices[0].pos = { -0.5f,0.5f ,0.0f };
	vertices[1].pos = { -0.5f,-0.5f ,0.0f };
	vertices[2].pos = { 0.5f,0.5f ,0.0f };
	vertices[3].pos = { 0.5f,-0.5f,0.0f };
	vertex[0].pos = vertices[0].pos;
	vertex[1].pos = vertices[1].pos;
	vertex[2].pos = vertices[2].pos;
	vertex[3].pos = vertices[3].pos;

	vertices[0].uv = { 0,1 };
	vertices[1].uv = { 0,0 };
	vertices[2].uv = { 1,1 };
	vertices[3].uv = { 1,0 };
	vertex[0].uv = vertices[0].uv;
	vertex[1].uv = vertices[1].uv;
	vertex[2].uv = vertices[2].uv;
	vertex[3].uv = vertices[3].uv;
	UnmapVertexBuffer();
}



void Sprite::CreateBuffer()
{

	//頂点バッファ作成
	CreateBuffer::GetInstance()->CreateVertexBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		sizeof(SpriteVertex),
		vertices.size(),
		vertexBufferSet
	);



	//定数バッファ作成
	CreateBuffer::GetInstance()->CreateConstBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		sizeof(SpriteConstBufferData),
		&constBuffer
	);
	
	


}

void Sprite::SetOneColorSpriteColor(const Color& color)
{
	SpriteConstBufferData* constBufferData;
	constBuffer->Map(0, nullptr, (void**)&constBufferData);

	constBufferData->color = DirectX::XMFLOAT4
	(
		static_cast<float>(color.r) / 255.0f,
		static_cast<float>(color.g) / 255.0f,
		static_cast<float>(color.b) / 255.0f,
		static_cast<float>(color.a) / 255.0f
	);

	constBuffer->Unmap(0, nullptr);
}

void Sprite::ConstDataMat()
{
	SpriteConstBufferData* constBufferData;
	constBuffer->Map(0, nullptr, (void**)&constBufferData);

	constBufferData->addColor = constData.addColor;
	constBufferData->subColor = constData.subColor;
	constBufferData->mulColor = constData.mulColor;


	constBuffer->Unmap(0, nullptr);
}

void Sprite::SetCmdList()
{
	DrawManager* drawManager = DrawManager::GetInstance();

	Texture* pTex = nullptr;
	if (drawMode == SpriteDrawMode::DRAW_TEXTURE)pTex = pTexture;

	

	//頂点
	//if (drawManager->GetPreDrawType() != DrawType::SPRITE)
	{
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		cmdList->SetGraphicsRootSignature(rootSignature.Get());
		cmdList->IASetVertexBuffers(0, 1, &vertexBufferSet.vertexBufferView);
	}
	cmdList->SetPipelineState(pipeline.Get());

	std::vector<ID3D12DescriptorHeap*> ppHeaps;
	ppHeaps.push_back(textureHeap.Get());
	cmdList->SetDescriptorHeaps(1, &ppHeaps[0]);

	//テクスチャ
	//if (drawManager->GetPreTexture()) 
	{
		UINT heapNum = 0;
		if (pTex)heapNum = pTex->GetTextureNumber();

		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
		gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			textureHeap->GetGPUDescriptorHandleForHeapStart(),
			heapNum,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);
	}

	//定数セット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer->GetGPUVirtualAddress());
	

	cmdList->DrawInstanced(vertices.size(), 1, 0, 0);
	
	drawManager->SetDrawData(*this);
}


void Sprite::MapVertexBuffer(void** data)
{
	vertexBufferSet.vertexBuffer->Map(0, nullptr, data);
}

void Sprite::UnmapVertexBuffer()
{
	vertexBufferSet.vertexBuffer->Unmap(0, nullptr);
}


void Sprite::Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmd)
{
	device = dev;
	cmdList = cmd;


#pragma region ヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = MAX_TEXTURE_LOAD_NUM;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	auto result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&textureHeap));
#pragma endregion

#pragma region 単色スプライト用の画像のバッファとビュー生成
	textureBuffer.resize(1);

	CreateBuffer::GetInstance()->CreateOneColorTextureBufferAndView
	(
		Color(0, 0, 0, 0),
		textureHeap->GetCPUDescriptorHandleForHeapStart(),
		&textureBuffer[0]
	);
#pragma endregion

#pragma region テーブル_ルートパラメーター

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootparam[2] = {};
	rootparam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparam[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);

#pragma endregion


#pragma region ルートシグネチャ


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparam;
	rootSignatureDesc.NumParameters = _countof(rootparam);


	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ComPtr<ID3DBlob>rootSigBlob;
	ComPtr<ID3DBlob>errorBlob;

	 result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);



	 if (FAILED(result))
	 {
		 std::string errstr;
		 errstr.resize(errorBlob->GetBufferSize());

		 std::copy_n((char*)errorBlob->GetBufferPointer(),
			 errorBlob->GetBufferSize(),
			 errstr.begin());
		 errstr += "\n";

		 OutputDebugStringA(errstr.c_str());
		 exit(1);
	 }

	result = device->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

#pragma endregion

	PipelineState::SetSpriteRootSignature(rootSignature.Get());
}

void Sprite::CreateTextureBuffer(Texture* texture)
{
	UINT textureNum = texture->GetTextureNumber();
	if(MAX_TEXTURE_LOAD_NUM <= textureNum)
	{
#ifdef _DEBUG

		std::wstring text = L"テクスチャの最大読み込み数("
			+ std::to_wstring(MAX_TEXTURE_LOAD_NUM)
			+ L")を超えました。これ以上テクスチャを読み込めません。\n";
		OutputDebugStringW(text.c_str());
#endif // _DEBUG
		return;
	}

	textureBuffer.resize(textureBuffer.size() + 1);

	D3D12_CPU_DESCRIPTOR_HANDLE hHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
	(
		textureHeap->GetCPUDescriptorHandleForHeapStart(),
		textureNum,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);

	CreateBuffer::GetInstance()->CreateSpriteTextureBufferAndView
	(
		texture->GetMetadata(),
		texture->GetImage()[0],
		hHandle,
		&textureBuffer[textureNum]
	);

	

}


void Sprite::SetPipeline(PipelineState* pipelineState)
{
	
	pipeline = pipelineState->GetPipelineState();
}


#pragma region 見た目操作
void Sprite::SetAddColor(const Color& color)
{
	constData.addColor =
	{
		(float)color.r / 255.0f,
		(float)color.g / 255.0f,
		(float)color.b / 255.0f,
		(float)color.a / 255.0f
	};
}

void Sprite::SetSubColor(const Color& color)
{
	constData.subColor =
	{
		(float)color.r / 255.0f,
		(float)color.g / 255.0f,
		(float)color.b / 255.0f,
		(float)color.a / 255.0f
	};
}

void Sprite::SetMulColor(const Color& color)
{
	constData.mulColor =
	{
		(float)color.r / 255.0f,
		(float)color.g / 255.0f,
		(float)color.b / 255.0f,
		(float)color.a / 255.0f
	};
}

#pragma endregion
