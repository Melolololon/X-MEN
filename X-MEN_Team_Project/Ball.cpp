#include "Ball.h"
#include "FieldObjectManager.h"
#include "FieldObjectWall.h"
#include "Enemy/FollowEnemy.h"
#include "Enemy/BarrierEnemy.h"
#include "NormalBarrier.h"
#include "EnemyBarrier.h"
#include "Enemy/EnemyManager.h"
#include <Random.h>
#include <LibMath.h>
#include <ModelData.cpp>

const MelLib::Color Ball::BALL_COLOR_RED = { 255,64,64,255 };
const MelLib::Color Ball::BALL_COLOR_BLUE = { 64,64,255,255 };
const MelLib::Color Ball::BALL_COLOR_BLUE2 = { 60,20,195,128 };
const MelLib::Color Ball::BALL_COLOR_YELLOW = { 255,255,64,255 };

void Ball::Move()
{
	//位置更新
	SetPosition(GetPosition() + velocity * speed);

	//スピード少し減らす
	speed -= BALL_FRICTION;
	if (speed < 0) { speed = 0; }
}

void Ball::SetColor(const MelLib::Color& color)
{
	modelObjects["main"].SetMulColor(color);
}

void Ball::Reflection(const Vector3& otherNormal, bool isAddSpeed)
{
	// 当たったオブジェクトの近くの座標を取得
	SetPosition(GetLerpExtrudePosition());

	//反射ベクトルを計算
	Vector3 reflectVel = (velocity - 2.0f * velocity.Dot(otherNormal) * otherNormal);

	//反映
	velocity = reflectVel;

	//位置更新 (めりこみ防止用)
	SetPosition(GetPosition() + velocity * speed);

	//加速
	if (isAddSpeed == false) {
		return;
	}

	speed += BALL_ACCEL;

	if (speed > MAX_SPEED)
	{
		speed = MAX_SPEED;
	}
}

Ball::Ball()
{
	// MelLib;;ModelObjectの配列
	MelLib::ModelData::Load("Resources/Model/Ball/Ball.obj", true, "objBall");
	modelObjects["main"].Create(MelLib::ModelData::Get("objBall"));
	modelObjects["main"].SetScale(scale);
	//青色セット
	SetColor(BALL_COLOR_YELLOW);
	throwingState = BallState::NONE;

	// 当たり判定の作成(球)
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(scale / 2);

	sphereFrameHitCheckNum = 4;

	//collisionCheckDistance = MAX_SCALE;

	SetPosition({ 20,0,0 });
}

Ball::~Ball()
{
	// 管理クラスから削除
	eraseManager = true;
}

void Ball::Update()
{
	//投げられていたら動かす
	if (isThrowed) {
		Move(); 
		AddScale();
		//停止
		if (speed <= 0) {
			////色セット
			//SetColor(BALL_COLOR_YELLOW);
			throwingState = BallState::NONE;
		}
	}
	else {
		//eraseManager = isPicked;
	}

	switch (throwingState)
	{
	case BallState::NONE:
		SetColor(BALL_COLOR_BLUE2);
		break;
	case BallState::HOLD_PLAYER:
		SetColor(BALL_COLOR_BLUE2);
		break;
	case BallState::HOLD_ENEMY:
		SetColor(BALL_COLOR_RED);
		break;
	case BallState::THROWING_PLAYER:
		SetColor(BALL_COLOR_BLUE2);
		break;
	case BallState::THROWING_ENEMY:
		SetColor(BALL_COLOR_RED);
		break;
	default:
		break;
	}
}

void Ball::Draw()
{
	AllDraw();
}

void Ball::Hit(const GameObject& object, const MelLib::ShapeType3D shapeType, const std::string& shapeName, const MelLib::ShapeType3D hitObjShapeType, const std::string& hitShapeName)
{
	//投げられていなければ衝突処理も発生しない
	if (isThrowed == false) {
		return;
	}

	//壁との衝突
	if (typeid(object) == typeid(FieldObjectWall))
	{
		//反射共通処理
		Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		Reflection(otherNormal, false);
	}
	//敵との衝突
	else if (typeid(object) == typeid(FollowEnemy) ||
		typeid(object) == typeid(BarrierEnemy))
	{
		// プレイヤーが投げたり反射させたボールなら
		if (throwingState == BallState::THROWING_PLAYER)
		{
			//反射共通処理
			Vector3 otherNormal = GetPosition() - object.GetPosition();
			otherNormal = otherNormal.Normalize();
			Reflection(otherNormal, true);

			throwingState = BallState::THROWING_ENEMY;
		}
	}
	//プレイヤーとの衝突
	else if (typeid(object) == typeid(Player))
	{
		//動いているときは反射
		if (speed > 0) {

			// 敵が投げたり反射させたボールなら
			if (throwingState == BallState::THROWING_ENEMY)
			{
				//反射共通処理
				Vector3 otherNormal = GetSphereCalcResult().GetBoxHitSurfaceNormal();
				Reflection(otherNormal, false);
			}
		}
	}
	//ノーマルバリアとの判定
	else if (typeid(object) == typeid(NormalBarrier))
	{

		//バリア展開中か取得するために型変換
		const NormalBarrier* other = static_cast<const NormalBarrier*>(&object);

		//バリア展開中でないならリターン
		if (other->GetIsOpen() == false) {
			return;
		}

		//反射共通処理
		Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		
		//プレイヤーのバリアに当たったらバリアの方向にそのまま返す
		velocity = MelLib::Quaternion(1,0,0,1).GetZXYRotateQuaternion({ 0,0,1 }, other->GetAngle()).ToVector3();
		//位置更新 (めりこみ防止用)
		SetPosition(GetPosition() + velocity * speed);

		//加速
		speed += BALL_ACCEL;

		if (speed > MAX_SPEED)
		{
			speed = MAX_SPEED;
		}

		throwingState = BallState::THROWING_PLAYER;
	}
	//エネミーバリアとの判定
	else if (typeid(object) == typeid(EnemyBarrier))
	{
		//バリア展開中か取得するために型変換
		const EnemyBarrier* other = static_cast<const EnemyBarrier*>(&object);

		//バリア展開中でないならリターン
		if (other->GetIsOpen() == false) {
			return;
		}

		//反射共通処理
		Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		Reflection(otherNormal, true);
		throwingState = BallState::THROWING_ENEMY;
	}
}

void Ball::ThrowBall(const Vector3& initVel)
{
	//スピードを初期値に
	speed = INIT_THROW_SPEED;

	//方向セット
	velocity = initVel;

	//射出フラグを有効に
	isThrowed = true;
}

void Ball::PickUp(const Vector3& ballPos, const MelLib::Color& initColor)
{
	//位置セット
	SetPosition(ballPos);

	//色セット
	SetColor(initColor);

	//投げたフラグオフ
	isThrowed = false;
}

void Ball::AddScale()
{
	if (EnemyManager::GetInstance()->GetIsDeadFlame() == false) {
		return;
	}

	//ひとまず5回敵倒すと最大値になるように
	scale += (MAX_SCALE - INIT_SCALE) / 5;

	if (scale > MAX_SCALE) {
		scale = MAX_SCALE;
	}

	//オブジェクトに反映
	SetScale({ scale,scale,scale });

	EnemyManager::GetInstance()->SetIsDeadFlame(false);
}
