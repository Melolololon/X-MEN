#include "SpriteFont2D.h"

//Spriteを途中でテクスチャ変えたり、べた塗りにしたりできるようにする
//そうしないとスプライトフォントのスプライトを先に用意できない

using namespace MelLib;
SpriteFont2D* SpriteFont2D::GetInstance()
{
	static SpriteFont2D inst;
	return &inst;
}

void SpriteFont2D::Initialize()
{
	for(auto& p : pSprites)
	{
		p = std::make_unique<Sprite2D>();
		p->Create(nullptr);
	}
}

void SpriteFont2D::Update()
{
	for(auto& spr : pSprites)
	{
		spr->SetTexture(nullptr);
	}
	spriteCount = 0;
}

bool SpriteFont2D::Draw(const Vector2& position, const Vector2& scale, const CharSequence sequence, const std::string& text, TextureFont* textureFont)
{

	if(!textureFont)
	{
#ifdef _DEBUG
		OutputDebugStringW(L"スプライトフォントの描画に失敗しました。textureFontがnullptrでした。\n");
#endif // _DEBUG
		return false;
	}

	Vector2 fontTextureSize = textureFont->GetPTexture()->GetTextureSize();
	Value2<UINT> fontNum = textureFont->GetFontLineNumber();
	Vector2 fontSize = Vector2(fontTextureSize.x / fontNum.v1, fontTextureSize.y / fontNum.v2);


	for (int i = 0,textSize = text.size(); i < textSize; i++)
	{
		if (spriteCount > MAX_SPRITE_NUM)
		{
#ifdef _DEBUG
			OutputDebugStringW(L"スプライトフォントの表示限界数(500文字)を越えたため、描画できません。\n");
#endif // _DEBUG
			return false;

		}

		Sprite2D* spr = pSprites[spriteCount].get();
		spr->SetTexture(textureFont->GetPTexture());

		char textChar = text[i] - 32;//32引かないといけない
		Value2<UINT>drawArea = Value2<UINT>(textChar, 0);
		while(drawArea.v1 >= fontNum.v1)
		{
			drawArea.v1 -= fontNum.v1;
			drawArea.v2++;
		}

		Vector2 drawAreaLeftUp = Vector2(fontSize.x * drawArea.v1, fontSize.y * drawArea.v2);
		//spr->SetDrawArea(drawAreaLeftUp, drawAreaLeftUp + fontSize);
		spr->SetDrawLeftUpPosition(drawAreaLeftUp);
		spr->SetDrawRigthDownPosition(drawAreaLeftUp + fontSize);

		Vector2 movePos = 0;
		switch (sequence)
		{
		case CharSequence::BESIDE:
			movePos = Vector2(fontSize.x, 0) * scale * i;
			break;
		case CharSequence::VERTICAL:
			movePos = Vector2(0, fontSize.y) * scale * i;
			break;
		}
		spr->SetPosition(position + movePos);


		Vector2 sprScale = 1.0f / Vector2(fontNum.v1, fontNum.v2) * scale;
		spr->SetScale(sprScale);

		spr->Draw();

		spriteCount++;
	}

	return true;
}
