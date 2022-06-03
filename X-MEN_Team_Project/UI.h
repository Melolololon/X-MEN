#pragma once
#include <Sprite2D.h>

class UI
{
protected:
	MelLib::Sprite2D sprite;
	std::shared_ptr<MelLib::Texture> pTexture;
	MelLib::Vector2 scale;
	MelLib::Vector2 angle;
	MelLib::Vector2 pos;
public:
	UI();
	virtual ~UI() {}
	virtual void Initialize() {}
	virtual void Update() {}
	virtual void Draw() {}

#pragma region Getter
	MelLib::Vector2 GetScale() const;
	MelLib::Vector2 GetPosition() const;
#pragma endregion

#pragma region Setter
	void SetTexture(std::shared_ptr<MelLib::Texture> setTexture);
	void SetScale(const MelLib::Vector2& setValue);
	void SetAngle(const MelLib::Vector2& setValue);
	void SetPosition(const MelLib::Vector2& setValue);
#pragma endregion
};
