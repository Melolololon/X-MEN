#pragma once
#include"Sprite2D.h"
#include"TextureFont.h"

namespace MelLib
{
	class SpriteFont2D
	{
	public:
		//ï∂éöÇÃï¿Ç—
		enum class CharSequence
		{
			BESIDE,//â°
			VERTICAL//èc
		};

	private:
		static const int MAX_SPRITE_NUM = 500;
		int spriteCount = 0;
		std::array<std::unique_ptr<Sprite2D>, MAX_SPRITE_NUM>pSprites;

		SpriteFont2D() {}
		~SpriteFont2D() {}
	public:
		SpriteFont2D(SpriteFont2D& spr) = delete;
		SpriteFont2D& operator=(SpriteFont2D& spr) = delete;
		static SpriteFont2D* GetInstance();

		void Initialize();
		void Update();

		bool Draw(const Vector2& position, const Vector2& scale, const CharSequence sequence, const std::string& text, TextureFont* textureFont);

	};
}
