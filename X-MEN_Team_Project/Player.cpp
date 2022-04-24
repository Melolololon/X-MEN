#include "Player.h"

#include<Input.h>

#include"TestObject.h"

void Player::Move()
{
	// 移動ベクトル
	MelLib::Vector3 moveVector;
	// 移動速度
	static const float MOVE_SPEED = 0.3f;
	
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
	// AddPosition、SetPositionは当たり判定も一緒に動く
	AddPosition(moveVector);
}

Player::Player()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// 初期位置を0,0,5に
	SetPosition(MelLib::Vector3(0, 0, 5));

	// 当たり判定の作成(球)
	// Playerの座標を取得し、それをセット
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);
}

void Player::Update()
{
	Move();

	// SPACE押したらモデル管理クラスから削除(メモリ解放)
	if(MelLib::Input::KeyTrigger(DIK_SPACE))
	{
		eraseManager = true;
	}

	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 255, 255));
}

void Player::Draw()
{
	// ModelObjectsに追加されているModelObjectをすべて描画
	AllDraw();
}

void Player::Hit
(
	const GameObject& object, 
	const MelLib::ShapeType3D shapeType, 
	const std::string& shapeName, 
	const MelLib::ShapeType3D hitObjShapeType,
	const std::string& hitShapeName
)
{
	// ここに当たった時の処理を記述
	// typeidなどで処理を分けたりする

	// テストオブジェクトと衝突したら色変更
	if(typeid(object) == typeid(TestObject))
	{
		modelObjects["main"].SetMulColor(MelLib::Color(100, 100, 100, 255));
	}
}
