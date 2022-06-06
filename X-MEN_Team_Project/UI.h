#pragma once
#include <Sprite2D.h>

class UI
{
protected:
	MelLib::Sprite2D sprite;
	MelLib::Texture* pTexture;
	MelLib::Vector2 scale;
	MelLib::Vector2 angle;
	MelLib::Vector2 pos;
protected:
	// スプライトをWindowのサイズに変更
	void ResizeWithWindow(const MelLib::Sprite2D& resizeSprite);
	// 引数のスプライトクラスにメンバ変数のスケール、アングル、ポジションをセットする
	void SetSpriteInfo(MelLib::Sprite2D& reinfoSprite,const MelLib::Vector2& setScale,float setAngle,const MelLib::Vector2& setPos);
public:
	UI(MelLib::Texture* pTexture);
	virtual ~UI() {}
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

#pragma region Getter
	MelLib::Vector2 GetScale() const;
	MelLib::Vector2 GetPosition() const;
#pragma endregion

#pragma region Setter
	void SetScale(const MelLib::Vector2& setValue);
	void SetAngle(const MelLib::Vector2& setValue);
	void SetPosition(const MelLib::Vector2& setValue);
#pragma endregion
};
