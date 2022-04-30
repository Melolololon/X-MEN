#include "Player.h"

#include<Input.h>
#include "InputDeviceManager.h"

#include"TestObject.h"

//void Player::Move()
//{
//	// 移動ベクトル
//	MelLib::Vector3 moveVector;
//	// 移動速度
//	static const float MOVE_SPEED = 0.3f;
//	
//	if (MelLib::Input::KeyState(DIK_LEFT))
//	{
//		moveVector.x -= MOVE_SPEED;
//	}
//	if (MelLib::Input::KeyState(DIK_RIGHT))
//	{
//		moveVector.x += MOVE_SPEED;
//	}
//	if (MelLib::Input::KeyState(DIK_UP))
//	{
//		moveVector.z += MOVE_SPEED;
//	}
//	if (MelLib::Input::KeyState(DIK_DOWN))
//	{
//		moveVector.z -= MOVE_SPEED;
//	}
//
//	// 斜め移動もちゃんとさせるため
//	moveVector = moveVector.Normalize() * MOVE_SPEED;
//
//	// 加算
//	// AddPosition、SetPositionは当たり判定も一緒に動く
//	AddPosition(moveVector);
//}

MelLib::Vector3 Player::GetInputVector()
{
	auto inputDeviceManager = InputDeviceManager::GetInstance();
	auto currentInputDevice = inputDeviceManager->GetCurrentInputDevice();
	MelLib::Vector3 moveVector;

	if (currentInputDevice == InputDeviceType::CONTROLLER)
	{
		moveVector = MelLib::Input::LeftStickVector3().Normalize();
		return moveVector;
	}

	if (currentInputDevice == InputDeviceType::KEYBOARD)
	{
		if (MelLib::Input::KeyState(DIK_A))
		{
			moveVector.x--;
		}
		if (MelLib::Input::KeyState(DIK_D))
		{
			moveVector.x++;
		}
		if (MelLib::Input::KeyState(DIK_W))
		{
			moveVector.z++;
		}
		if (MelLib::Input::KeyState(DIK_S))
		{
			moveVector.z--;
		}
	 
	 return moveVector.Normalize();
	}
	return MelLib::Vector3();
}

void Player::Move(const MelLib::Vector3& vec)
{
	static const float MOVE_SPEED = 0.7f;
	MelLib::Vector3 addVector = vec * MOVE_SPEED;
	AddPosition(addVector);
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

Player::~Player()
{
	// 管理クラスから削除
	eraseManager = true;
}

void Player::Update()
{
	Move(GetInputVector());

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
	if (typeid(object) == typeid(TestObject))
	{
		modelObjects["main"].SetMulColor(MelLib::Color(100, 100, 100, 255));
	}
}
