#include "Player.h"

#include<Input.h>
#include "InputDeviceManager.h"
#include"TestObject.h"
#include <GameObjectManager.h>
#include "FieldObjectWall.h"

MelLib::Vector3 Player::GetInputVector()
{
	auto inputDeviceManager = InputDeviceManager::GetInstance();
	auto currentInputDevice = inputDeviceManager->GetCurrentInputDevice();
	MelLib::Vector3 moveVector;
	bool isInput = false;

	// 現在の入力デバイスをもとに移動ベクトルを計算
	if (currentInputDevice == InputDeviceType::CONTROLLER)
	{
		moveVector = MelLib::Input::LeftStickVector3(InputDeviceInfo::STICK_START_ANGLE);
		isInput = MelLib::Input::LeftStickAngle(InputDeviceInfo::STICK_START_ANGLE) > 0 ? true : false;
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

	oldVelocity = addVector;

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
	if (!barrier)return;

	// バリアを使用中なら即終了
	if (barrier.get()->GetIsOpen())return;
	// ボールを保持しているならバリアは展開できない
	if(!pBall.get()->GetIsThrowed())return;

	barrier.get()->SetIsOpen(true);
}

void Player::ThrowingBall(bool key)
{
	if (!key)return;

	//ボールが投げられている場合はリターン
	if (pBall->GetIsThrowed()) {
		return;
	}

	//ボールを投げる
	pBall->ThrowBall(GetPosition());

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

void Player::TrackingBall()
{
	//ボールが投げられている場合はリターン
	if (pBall->GetIsThrowed()) {
		return;
	}

	//自分のちょっと右下に配置 (手に持ってるイメージ)
	pBall->SetBallPos(GetPosition() + MelLib::Vector3(0.25f, 0, -0.25f));
}

void Player::UpdateBarrierDirection()
{
	if (!barrier)return;

	barrier.get()->SetBarrierPosition(GetPosition(), dirVector);
}

Player::Player()
	: hp(PlayerInitializeInfo::MAX_HP)
	, ultimateSkillValue(0)
	, isThrowingBall(false)
	, ultimateSkill(UltimateSkill())
	, dirVector(MelLib::Vector3())
	, pBall(std::make_shared<Ball>())
	, barrier(nullptr)
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
	sphereDatas["main"][0].SetRadius(1);

	//ボールをオブジェクトマネージャに追加
	MelLib::GameObjectManager::GetInstance()->AddObject(pBall);
}

Player::~Player()
{
	// 管理クラスから削除
	eraseManager = true;
}

void Player::Update()
{
	Move(GetInputVector());

	TrackingBall();
	UpdateBarrierDirection();

	ultimateSkill.Update();

	// 各技処理を行う関数に対応したキーのトリガーを送って関数内で実行するか判断させる
	UseBarrier(MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A));
	ThrowingBall(MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A));
	UseUltimateSkill(MelLib::Input::KeyTrigger(DIK_Z) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::X));

	modelObjects["main"].SetMulColor(MelLib::Color(0, 0, 255, 255));
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

	// 壁とヒットしたときの押出処理
	if (typeid(object) == typeid(FieldObjectWall))
	{
		// ヒットした障害物のヒットした法線方向に押し出したいからその法線と
		// ヒット時の速度ベクトルを絶対値同士で乗算した後、もとの法線でかける
		MelLib::Vector3 otherNormal = GetSphereCalcResult().GetBoxHitSurfaceNormal();
		MelLib::Vector3 absOtherNormal;
		absOtherNormal.x = std::fabsf(otherNormal.x);
		absOtherNormal.y = std::fabsf(otherNormal.y);
		absOtherNormal.z = std::fabsf(otherNormal.z);

		MelLib::Vector3 absOldVelocity = oldVelocity;
		absOldVelocity.x = std::fabsf(absOldVelocity.x);
		absOldVelocity.y = std::fabsf(absOldVelocity.y);
		absOldVelocity.z = std::fabsf(absOldVelocity.z);

		MelLib::Vector3 reflectVector = absOtherNormal * absOldVelocity;
		reflectVector *= otherNormal;
		MelLib::Vector3 pos = GetPosition() + reflectVector;
		SetPosition(pos);
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
	if (!barrier)return false;

	return barrier.get()->GetIsOpen();
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

MelLib::Vector3 Player::GetBallPos() const
{
	return pBall.get()->GetPosition();
}

void Player::SetIsBarrier(bool flag)
{
	if (!barrier)return;
	barrier.get()->SetIsOpen(flag);
}

void Player::SetIsThrowingBall(bool flag)
{
	isThrowingBall = flag;
}

void Player::SetNormalBarrier(std::shared_ptr<NormalBarrier> setBarrier)
{
	barrier = setBarrier;
}
