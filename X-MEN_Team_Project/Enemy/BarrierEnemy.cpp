#include"BarrierEnemy.h"

#include"../Player.h"
#include<array>
#include"../MyLibrary/GameObjectManager.h"
#include "../FieldObjectWall.h"
#include <Random.h>

void BarrierEnemy::RefBallObject()
{
	if (refBallObject)return;
	// ボールの位置を設定
	for (const auto& v : MelLib::GameObjectManager::GetInstance()->GetRefGameObject()) {
		if (typeid(*v) == typeid(Ball)) {
			refBallObject = v;
			break;
		}
	}
}

void BarrierEnemy::SetRandomPosition()
{
	std::srand(std::rand());

	MelLib::Vector3 setPos;
	setPos.x = MelLib::Random::GetRandomNumberRangeSelect(-10, 10);
	setPos.z = MelLib::Random::GetRandomNumberRangeSelect(-10, 10);

	SetPosition(setPos);
}

BarrierEnemy::BarrierEnemy()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	const float MODEL_SIZE = 2;
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// 見た目がわかりやすいように 後程モデルデータができたときに修正
	modelObjects["main"].SetMulColor(MelLib::Color(255, 0, 255, 255));
	modelObjects["main"].SetScale(MODEL_SIZE);
	// 初期位置を0,0,5に
	SetPosition(MelLib::Vector3(0, 0, 5));

	// 当たり判定の作成(球)
	// Playerの座標を取得し、それをセット
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(MODEL_SIZE * 0.5f);

	// 変数の初期化
	hp = BarrierEnemyStatus::MAX_HP;
	playerDir = EnemyStatus::initPlayerDir;
	playerPos = EnemyStatus::initPlayerPos;
	// マジックナンバー?なので後で変更検討
	frontDir = { 0,0,0 };
	// 0で初期化
	ballDir.fill(0);

	ballCurrentNum = 0;
	ballBeforeNum = 0;
	firstCountflg = false;


	SetRandomPosition();
}

void BarrierEnemy::Move()
{
	// 移動ベクトル
	MelLib::Vector3 moveVector;
	// 移動速度
	static const float MOVE_SPEED = 0.3f;

	// 距離を計算し、一定距離以内なら外に出るように移動
	const float DISTANCE_X = GetPosition().x - playerPos.x;
	const float DISTANCE_Z = GetPosition().z - playerPos.z;

	float distance = sqrt(DISTANCE_X * DISTANCE_X + DISTANCE_Z * DISTANCE_Z);
	// 現在一定距離をlerpを使わず保つようになっている
	// 後程使用するように修正
	if (distance <= BarrierEnemyStatus::DISTANCE_TO_PLAYER)
	{

		moveVector = -playerDir * MOVE_SPEED;
	}

	else moveVector = { 0,0,0 };


	// 加算
	// AddPosition、SetPositionは当たり判定も一緒に動く


	AddPosition(moveVector);

	pastVelocity = moveVector;

	// 方向変換用
	if (firstCountflg)ChangePose();
}


void BarrierEnemy::ChangePose()
{
	const float PI = 3.1415926f;
	const float CALC_ANGLE = 180;

	// 方向ベクトルを元に向く方向を変更
	MelLib::Vector3 result;

	// atan2で方向ベクトルから計算
	result.y = atan2f(-ballDir[ballBeforeNum].z, ballDir[ballBeforeNum].x) * CALC_ANGLE / PI;

	SetAngle(result);

	// 正面ベクトルの書き換え
	frontDir = ballDir[ballBeforeNum];

	++ballBeforeNum;
	if (ballBeforeNum >= ballDir.size())
	{
		ballBeforeNum = 0;
	}

	// バリア用
	// 第二引数にとりあえずでボールへの方向ベクトル
	pBarrier.get()->SetBarrierPosition(GetPosition(), frontDir);

	
}

void BarrierEnemy::BallDirSort()
{
	// 前に配列を詰める
	for (int i = 0; i < ballDir.size() - 1; i++)
	{
		ballDir[i] = ballDir[i + 1];
	}

}

void BarrierEnemy::Update()
{
	RefBallObject();

	if (!refBallObject)return;

	Move();
	PushPosition();

	SetBallDir(refBallObject.get()->GetPosition());

	// hpがなくなったときに管理クラスから削除
	if (hp <= 0)
	{
		eraseManager = true;
	}

	modelObjects["main"].SetMulColor(MelLib::Color(255, 0, 255, 255));

}

void BarrierEnemy::Draw()
{
	// ModelObjectsに追加されているModelObjectをすべて描画
	AllDraw();
}

void BarrierEnemy::Hit(const GameObject& object, const MelLib::ShapeType3D shapeType, const std::string& shapeName, const MelLib::ShapeType3D hitObjShapeType, const std::string& hitShapeName)
{
	Enemy::Hit(object, shapeType, shapeName, hitObjShapeType, hitShapeName);
	// プレイヤーと衝突したら色変更　後程ボールと当たったときにも適用
	if (typeid(object) == typeid(Player))
	{
		modelObjects["main"].SetMulColor(MelLib::Color(100, 100, 100, 255));
	}

	// 壁との衝突判定
	if (typeid(object) == typeid(FieldObjectWall))
	{
		// ヒットした障害物のヒットした法線方向に押し出す
		MelLib::Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		MelLib::Vector3 pos = GetPosition() + -pastVelocity;
		SetPosition(pos);

		// 壁ずりベクトルを計算
		MelLib::Vector3 moveVector = pastVelocity - MelLib::Vector3Dot(pastVelocity, otherNormal) * otherNormal;
		moveVector *= pastVelocity.Length();

		AddPosition(moveVector);
	}
}

void BarrierEnemy::SetBallDir(const MelLib::Vector3& pos)
{
	// ボールの位置への方向ベクトルをとり変数にセット
	MelLib::Vector3 result = pos - GetPosition();

	ballDir[ballCurrentNum] = result.Normalize();
	++ballCurrentNum;
	if (ballCurrentNum >= ballDir.size())
	{
		ballCurrentNum = 0;
		firstCountflg = true;
	}
}

void BarrierEnemy::SetBarrier(std::shared_ptr<EnemyBarrier> barrier)
{
	pBarrier = barrier;
}
