#include "Player.h"

#include<Input.h>

void Player::Move()
{
	// 移動ベクトル
	MelLib::Vector3 moveVector;
	// 移動速度
	static const float MOVE_SPEED = 1.0f;
	
	if (MelLib::Input::KeyState(DIK_LEFT))
	{
		moveVector.x -= MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_RIGHT))
	{
		moveVector.x += MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_UP))
	{
		moveVector.z += MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_DOWN))
	{
		moveVector.z -= MOVE_SPEED;
	}

	// 加算
	AddPosition(moveVector);
}

Player::Player()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	SetPosition(MelLib::Vector3(0, 5, 0));
}

void Player::Update()
{
	Move();

	// SPACE押したらモデル管理クラスから削除(メモリ解放)
	if(MelLib::Input::KeyTrigger(DIK_SPACE))
	{
		eraseManager = true;
	}
}

void Player::Draw()
{
	// ModelObjectsに追加されているModelObjectをすべて描画
	AllDraw();
}

void Player::Hit
(
	const GameObject& object, 
	const MelLib::ShapeType3D& shapeType, 
	const std::string& shapeName, 
	const MelLib::ShapeType3D& hitObjShapeType, 
	const std::string& hitShapeName
)
{
	// ここに当たった時の処理を記述
	// typeidなどで処理を分けたりする
}
