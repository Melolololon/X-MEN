#include "Sprite3D.h"
#include"RenderTarget.h"

using namespace MelLib;
std::unordered_map<std::string, std::unique_ptr<Sprite3D>> Sprite3D::pSprite3D;
PipelineState Sprite3D::defaultPipeline;
DirectX::XMMATRIX Sprite3D::viewAndProjectionMatrix;
DirectX::XMFLOAT3 Sprite3D::cameraPosition;
DirectX::XMFLOAT3 Sprite3D::cameraTargetPosition;
DirectX::XMFLOAT3 Sprite3D::cameraUpVector;


void Sprite3D::Create(const Color& color, const std::string& name)
{
	pSprite3D.emplace(name, std::make_unique<Sprite3D>(color));
	pSprite3D[name]->Create(color);
}

void Sprite3D::Create(Texture* pTexture, const std::string& name)
{
	pSprite3D.emplace(name, std::make_unique<Sprite3D>(pTexture));
	pSprite3D[name]->Create(pTexture);
}

void Sprite3D::Delete(const std::string& name)
{
	pSprite3D.erase(name);
}

MelLib::Sprite3D::Sprite3D()
{
}

Sprite3D::Sprite3D(const Color& color)
{
	Create(color);
}


Sprite3D::Sprite3D(Texture* pTexture)
{
	Create(pTexture);
}

MelLib::Sprite3D::Sprite3D(const Sprite3D& sprite)
	: Sprite(sprite)
{
	billboardX = sprite.billboardX;
	billboardY = sprite.billboardY;
	billboardZ = sprite.billboardZ;


	if (sprite.vertexBufferSet.vertexBuffer)
	{
		if (pTexture)
		{
			Create(pTexture);
		}
		else
		{
			Create(color);
		}
	}

}

bool Sprite3D::Initialize()
{
	DrawData data;
	data.alphaWrite = true;
	data.blendMode = BlendMode::ADD;
	data.cullMode = CullMode::NONE;
	data.depthTest = true;
	data.drawMode = DrawMode::SOLID;
	
	ShaderDataSet set =
	{ 
		{ L"../MyLibrary/SpriteVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/SpritePixelShader.hlsl","PSmain","ps_5_0" }
	};
	
	auto result = defaultPipeline.CreatePipeline
	(
		data,
		set,
		PipelineStateType::SPRITE,
		nullptr,
		1
	);
	if (!result)
	{
		OutputDebugString(L"Sprite3Dの初期化に失敗しました。デフォルトパイプラインを生成できませんでした\n");
		return false;
	}
	return true;
}


void MelLib::Sprite3D::Create()
{
	CreateBuffer();
	InitializeVertices();
	if (!pTexture)
	{
		SetOneColorSpriteColor(color);
	}
	pipeline = defaultPipeline.GetPipelineState();
}

void Sprite3D::Create(const Color& color)
{
	CreateBuffer();
	InitializeVertices();
	SetOneColorSpriteColor(color);
	pipeline = defaultPipeline.GetPipelineState();
}

void Sprite3D::Create(Texture* pTexture)
{
	this->pTexture = pTexture;
	CreateBuffer();
	InitializeVertices();
	pipeline = defaultPipeline.GetPipelineState();

	if (pTexture)drawMode = SpriteDrawMode::DRAW_TEXTURE;
}

void Sprite3D::Draw(const std::string& rtName)
{
	RenderTarget::ChangeCurrentRenderTarget(RenderTarget::Get(rtName));
	
#pragma region map

	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);

#pragma region UV座標

	vertices[0].uv = { drawLeftUpPosition.x ,-drawRightDownPosition.y };
	vertices[1].uv = { drawLeftUpPosition.x,-drawLeftUpPosition.y };
	vertices[2].uv = { drawRightDownPosition.x ,-drawRightDownPosition.y };
	vertices[3].uv = { drawRightDownPosition.x ,-drawLeftUpPosition.y };
#pragma endregion
	auto vertexNum = vertices.size();
	for (int i = 0; i < vertexNum; i++)
	{
		vertex[i].uv = vertices[i].uv;
	}
	UnmapVertexBuffer();
#pragma endregion

	
	ConstDataMat();
	
	if (rtName != "") 
	{
		MatrixMap(RenderTarget::Get(rtName)->GetCamera());
		SetCmdList();
	}
	else
	{
		MatrixMap(RenderTarget::Get()->GetCamera());
		SetCmdList();
	}
}


void Sprite3D::MatrixMap(const Camera* camera)
{
	SpriteConstBufferData* constBufferData;
	constBuffer->Map(0, nullptr, (void**)&constBufferData);

	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	Billboard(matWorld);
	matWorld *= DirectX::XMMatrixScaling
	(
		constData.scale.x,
		constData.scale.y,
		1
	);
	matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(constData.angle.z));
	matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(constData.angle.x));
	matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(constData.angle.y));

	matWorld *= DirectX::XMMatrixTranslation
	(
		constData.position.x,
		constData.position.y,
		constData.position.z
	);


	constBufferData->mat = matWorld * camera->GetViewAndProjectionMat();


	constBuffer->Unmap(0, nullptr);
}

void Sprite3D::SetBillboardFlag(const bool flagX, const bool flagY, const bool flagZ)
{
	billboardX = flagX;
	billboardY = flagY;
	billboardZ = flagZ;
}

void Sprite3D::Billboard(DirectX::XMMATRIX& worldMat)
{
	if (!billboardX && !billboardY && !billboardZ)return;

	//回転させた座標の取得

	DirectX::XMVECTOR vCPos = DirectX::XMLoadFloat3(&cameraPosition);
	DirectX::XMVECTOR vCTarget = DirectX::XMLoadFloat3(&cameraTargetPosition);
	DirectX::XMVECTOR upVector = DirectX::XMLoadFloat3(&cameraUpVector);
	//Z軸を求める
	DirectX::XMVECTOR cameraAxisZ = DirectX::XMVectorSubtract(vCTarget, vCPos);

	//除外
	assert(!DirectX::XMVector3Equal(cameraAxisZ, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(cameraAxisZ));
	assert(!DirectX::XMVector3Equal(upVector, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(upVector));

	//ベクトルを正規化
	cameraAxisZ = DirectX::XMVector3Normalize(cameraAxisZ);

	//X軸を求める
	DirectX::XMVECTOR cameraAxisX = DirectX::XMVector3Cross(upVector, cameraAxisZ);

	//正規化
	cameraAxisX = DirectX::XMVector3Normalize(cameraAxisX);

	//Y軸を求める
	DirectX::XMVECTOR cameraAxisY = DirectX::XMVector3Cross(cameraAxisZ, cameraAxisX);

	//全方位ビルボード行列の計算
	DirectX::XMMATRIX billboardMatrix = DirectX::XMMatrixIdentity();
	if (billboardX)billboardMatrix.r[0] = cameraAxisX;
	if (billboardY)billboardMatrix.r[1] = cameraAxisY;
	if (billboardZ)billboardMatrix.r[2] = cameraAxisZ;
	billboardMatrix.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

	worldMat *= billboardMatrix;

}

