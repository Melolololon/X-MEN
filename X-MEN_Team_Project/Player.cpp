#include "Player.h"

#include<Input.h>
#include "InputDeviceManager.h"
#include"TestObject.h"
#include <GameObjectManager.h>
#include "FieldObjectWall.h"
#include <ImguiManager.h>
#include "Enemy/BarrierEnemy.h"
#include "Enemy/FollowEnemy.h"

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
	static const float MOVE_SPEED = 0.5f;
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

bool Player::UseBarrier(bool key)
{
	if (!key)return false;
	if (!barrier)return false;
	if (!pBall)return false;

	// バリアを使用中なら即終了
	if (barrier.get()->GetIsOpen())return false;

	barrier.get()->SetIsOpen(true);
	return true;
}

bool Player::ThrowingBall(bool key)
{
	if (!key)return false;

	if (pBall == nullptr)return false;

	//ボールが投げられている場合はリターン
	if (pBall->GetIsThrowed()) {
		return false;
	}

	//ボールを投げる
	pBall->SetPosition(GetPosition() + dirVector * 2);
	pBall->ThrowBall(dirVector);

	pBall.get()->SetThrowingState(BallState::THROWING_PLAYER);

	isThrowingBall = true;

	return true;
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
	if (pBall == nullptr)return;

	//ボールが投げられている場合はリターン
	if (pBall->GetIsThrowed()) {
		return;
	}

	MelLib::Vector3 rightVector = { -dirVector.z,0,dirVector.x };
	MelLib::Vector3 addVector = dirVector + rightVector;
	addVector /= 10;

	//自分のちょっと右下に配置 (手に持ってるイメージ)
	pBall->SetPosition(GetPosition() + addVector);
}

void Player::UpdateBarrierDirection()
{
	if (!barrier)return;

	barrier.get()->SetBarrierPosition(GetPosition(), dirVector);
}

void Player::UseAbility(bool key)
{
	if (!key)return;

	// ボールをプレイヤーが保持している状態かで分岐
	if (pBall.get()->GetThrowingState() == BallState::HOLD_PLAYER)
	{
		ThrowingBall(key);
	}
	else
	{
		UseBarrier(key);
	}
}

void Player::Knockback(const MelLib::Vector3& vector)
{
	const int POWER = 3;

	SetPosition(GetPosition() + vector * POWER);
}

void Player::UpdateIsThrowing(const float PER_FRAME)
{
	if (!isThrowingBall)return;

	if (throwingElapsedTime >= PlayerInitializeInfo::MAX_THROWING_TIME)
	{
		throwingElapsedTime = 0;
		isThrowingBall = false;
	}

	throwingElapsedTime += PER_FRAME;
}

Player::Player()
	: hp(PlayerInitializeInfo::MAX_HP)
	, ultimateSkillValue(0)
	, isThrowingBall(false)
	, throwingElapsedTime(0)
	, ultimateSkill(UltimateSkill())
	, dirVector(MelLib::Vector3())
	, pBall(nullptr)
	, barrier(nullptr)
	, hpGauge(GaugeUI())
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	const float MODEL_SIZE = 2;
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	modelObjects["main"].SetScale({ MODEL_SIZE,MODEL_SIZE,MODEL_SIZE });
	// 初期位置を0,0,5に
	SetPosition(MelLib::Vector3(0, 0, -10));

	// 当たり判定の作成(球)
	// Playerの座標を取得し、それをセット
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(MODEL_SIZE * 0.5f);

	hpGauge.SetFrontColor(PlayerHPUIInfo::FRONT_COLOR);
	hpGauge.SetBackColor(PlayerHPUIInfo::BACK_COLOR);
	hpGauge.SetMaxValue(PlayerInitializeInfo::MAX_HP);
	hpGauge.SetPosition(PlayerHPUIInfo::DRAW_POSITION);
	hpGauge.SetSizePercent(PlayerHPUIInfo::SIZE_PERCENT);
}

Player::~Player()
{
	// 管理クラスから削除
	eraseManager = true;
}

void Player::Update()
{
	bool isInputAbilityKey = MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A);

	Move(GetInputVector());

	TrackingBall();
	UpdateBarrierDirection();

	ultimateSkill.Update();

	// 各技処理を行う関数に対応したキーのトリガーを送って関数内で実行するか判断させる
	UseAbility(isInputAbilityKey);
	UseUltimateSkill(MelLib::Input::KeyTrigger(DIK_Z) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::X));

	modelObjects["main"].SetMulColor(MelLib::Color(0, 0, 255, 255));

	hpGauge.Update(hp);

	const float PER_FRAME = 1.0f / 60.0f;
	UpdateIsThrowing(PER_FRAME);
}

void Player::Draw()
{
	// ModelObjectsに追加されているModelObjectをすべて描画
	AllDraw();
	// 必殺技ゲージの描画
	ultimateSkill.Draw();

	hpGauge.Draw();
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
	if (typeid(object) == typeid(TestObject)) {
		modelObjects["main"].SetMulColor(MelLib::Color(100, 100, 100, 255));
	}
	else if (typeid(object) == typeid(Ball)) {
		//スピード取得のために型変換
		const Ball* other = static_cast<const Ball*>(&object);

		if (!pBall)return;
		if (isThrowingBall)return;

		BallState ballThrowingState = pBall.get()->GetThrowingState();
		switch (ballThrowingState)
		{
		case BallState::NONE:
			pBall.get()->PickUp(GetPosition() + MelLib::Vector3(0.25f, 0, -0.25f), Ball::BALL_COLOR_BLUE2);
			pBall.get()->SetThrowingState(BallState::HOLD_PLAYER);
			break;
		case BallState::THROWING_PLAYER:
			pBall.get()->PickUp(GetPosition() + MelLib::Vector3(0.25f, 0, -0.25f), Ball::BALL_COLOR_BLUE2);
			pBall.get()->SetThrowingState(BallState::HOLD_PLAYER);
			break;
		case BallState::THROWING_ENEMY:
			Damage(pBall.get()->GetSpeed());

			MelLib::Vector3 knockbackVector = GetPosition() - pBall.get()->GetPosition();
			Knockback(knockbackVector.Normalize());
			break;
		}
	}

	// 壁とヒットしたときの押出処理
	if (typeid(object) == typeid(FieldObjectWall))
	{
		// ヒットした障害物のヒットした法線方向に押し出したいからその法線を取得
		MelLib::Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		MelLib::Vector3 pos = GetPosition() + -oldVelocity;
		SetPosition(pos);

		// 壁ずりベクトルを計算してAddPosition
		MelLib::Vector3 moveVector = oldVelocity - MelLib::Vector3Dot(oldVelocity, otherNormal) * otherNormal;
		moveVector *= oldVelocity.Length();
		AddPosition(moveVector);
	}

	if (typeid(object) == typeid(FollowEnemy) ||
		typeid(object) == typeid(BarrierEnemy))
	{
		const Enemy* other = static_cast<const Enemy*>(&object);
		MelLib::Vector3 knockbackVector = GetPosition() - other->GetPosition();

		Damage(PlayerDamageInfo::HIT_ENEMY_DAMAGE);
		Knockback(knockbackVector.Normalize());
	}
}

void Player::AddUltimatekillValue(int value)
{
	ultimateSkillValue += value;
}

void Player::Damage(float value)
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

const MelLib::Vector3& Player::GetDirection() const
{
	return dirVector;
}

float Player::GetHp() const
{
	return hp;
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

void Player::SetBall(std::shared_ptr<Ball> setBall)
{
	pBall = setBall;
}
