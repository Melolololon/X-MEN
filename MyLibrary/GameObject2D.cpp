#include "GameObject2D.h"

void MelLib::GameObject2D::MoveSpritePosition(const Vector2& vec)
{
	for(auto& spr : sprites)
	{
		spr.second.SetPosition(spr.second.GetPosition() + vec);
	}
}

void MelLib::GameObject2D::MoveDataPosition(const Vector2& vec)
{
	for (auto& data : circleDatas)
	{
		data.SetPosition(data.GetPosition() + vec);
	}
	for (auto& data : rectDatas)
	{
		data.SetPosition(data.GetPosition() + vec);
	}
	for (auto& data : segmentDatas)
	{
		data.SetPosition(data.GetPosition() + vec);
	}
}

MelLib::GameObject2D::GameObject2D()
{
}

MelLib::GameObject2D::~GameObject2D()
{
}

void MelLib::GameObject2D::Update()
{
}

void MelLib::GameObject2D::Draw()
{
}

void MelLib::GameObject2D::Hit
(
	const GameObject2D* const pObject,
	const ShapeType2D& dataType,
	const int num,
	const ShapeType2D& hitObjDataType,
	const int hitNum
)
{
}

void MelLib::GameObject2D::AddPosition(const MelLib::Vector2& vec)
{
	Vector2 prePos = position;
	position += vec;
	Vector2 preToPos = position - prePos;
	MoveSpritePosition(preToPos);
	MoveDataPosition(preToPos);
}

void MelLib::GameObject2D::FalseEraseManager()
{
	eraseManager = false;
}

void MelLib::GameObject2D::SetPosition(const MelLib::Vector2& pos)
{
	Vector2 prePos = position;
	position = pos;
	Vector2 preToPos = position - prePos;
	MoveSpritePosition(preToPos);
	MoveDataPosition(preToPos);
}
