#include "Player.h"

#include<Input.h>
#include "InputDeviceManager.h"
#include"TestObject.h"

MelLib::Vector3 Player::GetInputVector()
{
	auto inputDeviceManager = InputDeviceManager::GetInstance();
	auto currentInputDevice = inputDeviceManager->GetCurrentInputDevice();
	MelLib::Vector3 moveVector;
	bool isInput = false;

	// 現在の入力デバイスをもとに移動ベクトルを計算
	if (currentInputDevice == InputDeviceType::CONTROLLER)
	{
		moveVector = MelLib::Input::LeftStickVector3();
		isInput = MelLib::Input::LeftStickAngle() > 0 ? true : false;
	}

	if (currentInputDevice == InputDeviceType::KEYBOARD)
	{
		if (MelLib::Input::KeyState(DIK_A))
		{
			moveVector.x--;
			isInput = true;
		}
		if (MelLib::Input::KeyState(DIK_D))
		{
			moveVector.x++;
			isInput = true;
		}
		if (MelLib::Input::KeyState(DIK_W))
		{
			moveVector.z++;
			isInput = true;
		}
		if (MelLib::Input::KeyState(DIK_S))
		{
			moveVector.z--;
			isInput = true;
		}
	}
	
	// 移動キーを入力しているなら方向ベクトルを更新
	if (isInput)dirVector = moveVector.Normalize();

	return moveVector.Normalize();
}

void Player::Move(const MelLib::Vector3& vec)
{
	static const float MOVE_SPEED = 0.7f;
	MelLib::Vector3 addVector = vec * MOVE_SPEED;

	GameObject::AddPosition(addVector);
	CalclateDirection();
}

void Player::CalclateDirection()
{
	const float PI = 3.141592f;
	const float MAX_ANGLE = 180.0f;

	// 方向ベクトルをもとに回転値を計算
	float angleY = MAX_ANGLE / PI * std::atan2f(-dirVector.z, dirVector.x);
	GameObject::SetAngle(MelLib::Vector3(0, angleY, 0));
}

void Player::UseBarrier(bool key)
{
	if (!key)return;

	// バリアを使用中なら即終了
	if (isBarrier)return;
	//// ボールを保持しているならバリアは展開できない
	//if(ball)return;

	isBarrier = true;
}

void Player::ThrowingBall(bool key)
{
	if (!key)return;

	//// ボールを保持していないなら終了
	//if(!ball)return;

	isThrowingBall = true;
}

void Player::UseUltimateSkill(bool key)
{
	if (!key)return;

	// スキル使用中なら即終了
	if (ultimateSkill.GetIsUsingSkill())return;

	// スキル使用
	ultimateSkill.Use();
}

Player::Player()
	: hp(PlayerInitializeInfo::MAX_HP)
	, ultimateSkillValue(0)
	, isBarrier(false)
	, isThrowingBall(false)
	, ultimateSkill(UltimateSkill())
	, dirVector(MelLib::Vector3())
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

	ultimateSkill.Update();

	if (MelLib::Input::KeyTrigger(DIK_SPACE))ultimateSkill.AddValue(10);

	// 各技処理を行う関数に対応したキーのトリガーを送って関数内で実行するか判断させる
	UseBarrier(MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A));
	ThrowingBall(MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A));
	UseUltimateSkill(MelLib::Input::KeyTrigger(DIK_Z) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::X));

	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 255, 255));
}

void Player::Draw()
{
	// ModelObjectsに追加されているModelObjectをすべて描画
	AllDraw();
	// 必殺技ゲージの描画
	ultimateSkill.Draw();
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

void Player::AddUltimatekillValue(int value)
{
	ultimateSkillValue += value;
}

void Player::Damage(int value)
{
	hp -= value;
}

bool Player::GetIsBarrier() const
{
	return isBarrier;
}

bool Player::GetIsThrowingBall() const
{
	return isThrowingBall;
}

bool Player::GetIsUltimateSkill() const
{
	return ultimateSkill.GetIsUsingSkill();
}

MelLib::Vector3 Player::GetDirection() const
{
	return dirVector;
}

void Player::SetIsBarrier(bool flag)
{
	isBarrier = flag;
}

void Player::SetIsThrowingBall(bool flag)
{
	isThrowingBall = flag;
}
