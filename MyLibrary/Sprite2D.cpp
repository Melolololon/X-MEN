#include "Sprite2D.h"
#include"RenderTarget.h"

using namespace MelLib;

std::unordered_map<std::string, std::unique_ptr<Sprite2D>> Sprite2D::pSprite2D;

Sprite2D::Sprite2D()
{
}

Sprite2D::Sprite2D(const Color& color)
{
	Create(color);
}

Sprite2D::Sprite2D(Texture* pTexture)
{
	Create(pTexture);
}

Sprite2D::~Sprite2D()
{
}

void MelLib::Sprite2D::Create()
{
	SpriteInitialize();

	if(pTexture)
	{
		// テクスチャに合わせてサイズ変更
		drawRightDownPosition = pTexture->GetTextureSize();

		SetColor(Color(0, 0, 0, 0));

		drawMode = SpriteDrawMode::DRAW_TEXTURE;
	}
	else 
	{
		SetOneColorSpriteColor(color);
		drawMode = SpriteDrawMode::DRAW_COLOR;
	}
	pipeline = defaultPipeline.GetPipelineState();
}

void Sprite2D::Create(const Color& color)
{
	SpriteInitialize();
	SetOneColorSpriteColor(color);
	drawMode = SpriteDrawMode::DRAW_COLOR;

	pipeline = defaultPipeline.GetPipelineState();
}

void Sprite2D::Create(Texture* pTexture)
{
	if (pTexture)
	{
		// テクスチャに合わせてサイズ変更
		drawRightDownPosition = pTexture->GetTextureSize();
	}
	this->pTexture = pTexture;
	

	SpriteInitialize();
	SetColor(Color(0, 0, 0, 0));

	drawMode = SpriteDrawMode::DRAW_TEXTURE;
	pipeline = defaultPipeline.GetPipelineState();
}
//
//bool Sprite2D::Create(const Color& color, const std::string& name)
//{
//	pSprite2D.emplace(name, std::make_unique<Sprite2D>());
//	bool result = pSprite2D[name]->Create(color);
//
//#ifdef _DEBUG
//	if (!result)
//	{
//		OutputDebugStringA(name.c_str());
//		OutputDebugStringW(L"の生成に失敗しました。\n");
//	}
//#endif // _DEBUG
//
//	return result;
//
//}
//
//bool Sprite2D::Create(Texture* pTexture, const std::string& name)
//{
//	pSprite2D.emplace(name, std::make_unique<Sprite2D>());
//	bool result = pSprite2D[name]->Create(pTexture);
//
//#ifdef _DEBUG
//	if(!result)
//	{
//		OutputDebugStringA(name.c_str());
//		OutputDebugStringW(L"の生成に失敗しました。\n");
//	}
//#endif // _DEBUG
//
//	return result;
//}

void Sprite2D::Delete(const std::string& name)
{
	pSprite2D.erase(name);
}

void Sprite2D::Draw(const std::string& rtName)
{
	RenderTarget::ChangeCurrentRenderTarget(RenderTarget::Get(rtName));

	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);

	Vector2 textureSize = 1;
	if (pTexture) textureSize = pTexture->GetTextureSize();

#pragma region 頂点座標

	MelLib::Vector2 areaSize = drawRightDownPosition - drawLeftUpPosition;

	//vertices[0].pos = { -textureSize.x / 2 , textureSize.y / 2, 0 };
	//vertices[1].pos = { -textureSize.x / 2 ,-textureSize.y / 2,0 };
	//vertices[2].pos = { textureSize.x / 2  ,   textureSize.y / 2 ,0 };
	//vertices[3].pos = { textureSize.x / 2  , -textureSize.y / 2,0 };

	// 頂点座標に表示範囲を適応
	vertices[0].pos = { -areaSize.x / 2 , areaSize.y / 2, 0 };
	vertices[1].pos = { -areaSize.x / 2 ,-areaSize.y / 2,0 };
	vertices[2].pos = { areaSize.x / 2  ,   areaSize.y / 2 ,0 };
	vertices[3].pos = { areaSize.x / 2  , -areaSize.y / 2,0 };
#pragma endregion

#pragma region UV座標


	Vector2 uvLeftUp = { 1.0f / textureSize.x * drawLeftUpPosition.x ,1.0f / textureSize.y * drawLeftUpPosition.y };
	Vector2 uvRightDown = { 1.0f / textureSize.x * drawRightDownPosition.x ,1.0f / textureSize.y * drawRightDownPosition.y };

	vertices[0].uv = { uvLeftUp.x ,uvRightDown.y };
	vertices[1].uv = { uvLeftUp.x,uvLeftUp.y };
	vertices[2].uv = { uvRightDown.x ,uvRightDown.y };
	vertices[3].uv = { uvRightDown.x ,uvLeftUp.y };
#pragma endregion


	auto vertexNum = vertices.size();
	for (int i = 0; i < vertexNum; i++)
		vertex[i] = vertices[i];

	UnmapVertexBuffer();

	ConstDataMat();
	
	Texture* pTex = pTexture;
	if (drawMode == SpriteDrawMode::DRAW_COLOR)pTex = nullptr;
	MatrixMap(pTex);
	SetCmdList();
}

void Sprite2D::SetColor(const Color& color)
{
	this->color = color;
	SetOneColorSpriteColor(color);
}

void Sprite2D::SetTexture(Texture* pTexture)
{
	this->pTexture = pTexture;
	
	if (pTexture) 
	{
		SetDrawRigthDownPosition(pTexture->GetTextureSize());
		SetDrawMode(SpriteDrawMode::DRAW_TEXTURE);
	}
}
