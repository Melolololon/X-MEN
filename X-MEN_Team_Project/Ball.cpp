#include "Ball.h"
#include "FieldObjectManager.h"
#include "FieldObjectWall.h"
#include "Enemy/FollowEnemy.h"
#include "Enemy/BarrierEnemy.h"
#include "NormalBarrier.h"
#include "EnemyBarrier.h"
#include <Random.h>
#include <LibMath.h>
#include <GameObjectManager.h>

const MelLib::Color Ball::BALL_COLOR_RED = { 255,64,64,255 };
const MelLib::Color Ball::BALL_COLOR_BLUE = { 64,64,255,255 };
const MelLib::Color Ball::BALL_COLOR_BLUE2 = { 60,20,195,255 };
const MelLib::Color Ball::BALL_COLOR_YELLOW = { 255,255,64,255 };

void Ball::Move()
{
	//位置更新
	SetPosition(GetPosition() + velocity * speed);

	//スピード少し減らす
	speed -= 0.0025f;
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

const MelLib::Color Ball::GetColorFromBallState(const BallState& ballState)
{
	MelLib::Color result;
	switch (throwingState)
	{
	case BallState::NONE:
		result = BALL_COLOR_BLUE2;
		break;
	case BallState::HOLD_PLAYER:
		result = BALL_COLOR_BLUE2;
		break;
	case BallState::HOLD_ENEMY:
		result = BALL_COLOR_RED;
		break;
	case BallState::THROWING_PLAYER:
		result = BALL_COLOR_BLUE2;
		break;
	case BallState::THROWING_ENEMY:
		result = BALL_COLOR_RED;
		break;
	default:
		result = BALL_COLOR_BLUE2;
		break;
	}

	return result;
}

Ball::Ball()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	const float SCALE = 2;
	const float MODEL_SIZE = 2 * SCALE;
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	SetScale(MODEL_SIZE);

	throwingState = BallState::NONE;
	SetColor(GetColorFromBallState(throwingState));

	// 当たり判定の作成(球)
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(MODEL_SIZE*0.5f);

	sphereFrameHitCheckNum = 4;

	SetPosition({ 0,0,-10 });

	//軌跡オブジェクト生成
	for (auto& v : pBallTrajectories) {
		v = std::make_shared<BallTrajectory>();
		//自分に紐づいているモデルを引っ張ってくる
		v->CreateModel(modelObjects["main"].GetPModelData());
		//初期位置＝自分の位置
		v->SetPosition(GetPosition());
		//スケールセット
		v->SetScale(GetScale());
		//色セット
		v->SetMulColor(GetColorFromBallState(throwingState));
		//追加
		MelLib::GameObjectManager::GetInstance()->AddObject(v);
	}
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

	//軌跡更新
	static int a = 0;
	a++;
	if (a == 5) {
		UpdateTrajectories();
		a = 0;
	}

	//投げられている状態から色セット
	SetColor(GetColorFromBallState(throwingState));
}

void Ball::Draw()
{
	//まず軌跡描画
	DrawTrajectories();
	//ボール本体描画
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

	//位置を移動方向へオブジェクトの大きさだけずらす (投げた瞬間衝突するのを防ぐ)
	SetPosition(GetPosition() + velocity * GetScale());

	//射出フラグを有効に
	isThrowed = true;

	//軌跡オブジェクトの情報を全て今のボールの情報に
	for (auto& v : pBallTrajectories) {
		//自分の位置
		v->SetPosition(GetPosition());
		//スケールセット
		v->SetScale(GetScale());
		//色セット
		v->SetMulColor(GetColorFromBallState(throwingState));
	}
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

void Ball::UpdateTrajectories()
{
	//一番後ろのオブジェクトから本体に向かって情報をコピーしていく
	for (int i = _countof(pBallTrajectories) - 1; i >= 0; i--) {
		if (i > 0) {
			pBallTrajectories[i]->SetPosition(pBallTrajectories[i - 1]->GetPosition());	//座標
			pBallTrajectories[i]->SetScale(pBallTrajectories[i - 1]->GetScale());		//スケール
			MelLib::Color color = pBallTrajectories[i - 1]->GetColor();
			color.a = 64;
			pBallTrajectories[i]->SetColor(color);		//色
		}
		else {
			//一番本体に近いオブジェクトは本体の情報コピー
			pBallTrajectories[i]->SetPosition(GetPosition());							//座標
			pBallTrajectories[i]->SetScale(GetScale());									//スケール
			MelLib::Color color = GetColorFromBallState(throwingState);
			color.a = 64;
			pBallTrajectories[i]->SetColor(color);		//色
		}
	}
}

void Ball::DrawTrajectories()
{
	for (auto& v : pBallTrajectories) {
		v->Draw();
	}
}

void BallTrajectory::Update()
{
}

void BallTrajectory::Draw()
{
	AllDraw();
}

void BallTrajectory::CreateModel(MelLib::ModelData* pModelData)
{
	modelObjects["main"].Create(pModelData);
}

void BallTrajectory::SetColor(const MelLib::Color& color)
{
	this->color = color;
	modelObjects["main"].SetMulColor(color);
}