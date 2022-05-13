#include "Ball.h"
#include "FieldObjectManager.h"
#include "FieldObjectWall.h"
#include "Enemy/FollowEnemy.h"
#include "Enemy/BarrierEnemy.h"
#include "NormalBarrier.h"
#include "EnemyBarrier.h"
#include <Random.h>
#include <LibMath.h>

const MelLib::Color Ball::BALL_COLOR_RED = { 255,64,64,255 };
const MelLib::Color Ball::BALL_COLOR_BLUE = { 64,64,255,255 };
const MelLib::Color Ball::BALL_COLOR_YELLOW = { 255,255,64,0 };

void Ball::Move()
{
	//位置更新
	SetPosition(GetPosition() + velocity * speed);

	//スピード少し減らす
	speed -= 0.001f;
	if (speed < 0) { speed = 0; }
}

void Ball::SetColor(const MelLib::Color& color)
{
	modelObjects["main"].SetMulColor(color);
}

void Ball::Reflection(const Vector3& otherNormal)
{
	//反射ベクトルを計算
	Vector3 reflectVel = (velocity - 2.0f * velocity.Dot(otherNormal) * otherNormal);

	//反映
	velocity = reflectVel;

	//位置更新 (めりこみ防止用)
	SetPosition(GetPosition() + velocity * speed);

	//加速
	speed += BALL_ACCEL;
}

Ball::Ball()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	//青色セット
	SetColor(BALL_COLOR_YELLOW);
	throwingState = BallState::NONE;

	// 当たり判定の作成(球)
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(1);
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
		SetColor(BALL_COLOR_BLUE);
		break;
	case BallState::HOLD_PLAYER:
		SetColor(BALL_COLOR_BLUE);
		break;
	case BallState::HOLD_ENEMY:
		SetColor(BALL_COLOR_RED);
		break;
	case BallState::THROWING_PLAYER:
		SetColor(BALL_COLOR_BLUE);
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
		Vector3 otherNormal = GetSphereCalcResult().GetBoxHitSurfaceNormal();
		Reflection(otherNormal);
	}
	//敵との衝突
	else if (typeid(object) == typeid(FollowEnemy) ||
		typeid(object) == typeid(BarrierEnemy))
	{
		// プレイヤーが投げたり反射させたボールなら
		if (throwingState == BallState::THROWING_PLAYER)
		{
			//反射共通処理
			Vector3 otherNormal = GetSphereCalcResult().GetBoxHitSurfaceNormal();
			Reflection(otherNormal);
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
				Reflection(otherNormal);
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
		Reflection(otherNormal);
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
		Reflection(otherNormal);
		throwingState = BallState::THROWING_ENEMY;
	}
}

void Ball::ThrowBall(const Vector3& initVel)
{
	//スピードを初期値に
	speed = INIT_THROW_SPEED;

	//方向セット
	velocity = initVel;

	//位置を移動方向へオブジェクトの大きさだけずらす (投げた瞬間衝突するのを防ぐ)
	SetPosition(GetPosition() + velocity * GetScale());

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