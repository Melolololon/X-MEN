#include"BarrierEnemy.h"

#include"../Player.h"


BarrierEnemy::BarrierEnemy()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// 見た目がわかりやすいように 後程モデルデータができたときに修正
	modelObjects["main"].SetMulColor(MelLib::Color(255, 0, 255, 255));

	// 初期位置を0,0,5に
	SetPosition(MelLib::Vector3(0, 0, 5));

	// 当たり判定の作成(球)
	// Playerの座標を取得し、それをセット
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);

	// 変数の初期化
	hp = BarrierEnemyStatus::MAX_HP;
	playerDir = EnemyStatus::initPlayerDir;
	playerPos = EnemyStatus::initPlayerPos;
	// マジックナンバーではあるので後で変更検討
	ChangePoseFrameCount = 0;
	ballDir = { 0,0,0 };

}

void BarrierEnemy::Move()
{
	// 移動ベクトル
	MelLib::Vector3 moveVector;
	// 移動速度
	static const float MOVE_SPEED = 0.7f;

	// 距離を計算し、一定距離以内なら外に出るように移動
	const float DISTANCE_X = GetPosition().x - playerPos.x;
	const float DISTANCE_Z = GetPosition().z - playerPos.z;

	float distance = sqrt(DISTANCE_X * DISTANCE_X + DISTANCE_Z * DISTANCE_Z);
	// 現在一定距離をlerpを使わず保つようになっている
	// 後程使用するように修正
	if (distance <= BarrierEnemyStatus::DISTANCE_TO_PLAYER)moveVector = -playerDir * MOVE_SPEED;
	else moveVector = { 0,0,0 };


	// 加算
	// AddPosition、SetPositionは当たり判定も一緒に動く
	AddPosition(moveVector);


	// 方向変換用
	ChangePose();

	// バリア用
	// 第二引数にとりあえずでボールへの方向ベクトル
	pBarrier.get()->SetBarrierPosition(GetPosition(), ballDir);
}


void BarrierEnemy::ChangePose()
{

	MelLib::Vector3 temp = GetAngle();

	const float PI = 3.1415926f;
	const float CALC_ANGLE = 180;

	// 方向ベクトルを元に向く方向を変更
	
	
	// 前の角度を取得
	MelLib::Vector3 angle = GetAngle();

	MelLib::Vector3 result;

	// atan2で方向ベクトルから計算
	result.x = atan2f(ballDir.x, ballDir.y) * CALC_ANGLE / PI;
	result.y = atan2f(-ballDir.z, ballDir.x) * CALC_ANGLE / PI;
	result.z = atan2f(ballDir.y, -ballDir.z) * CALC_ANGLE / PI;

	SetAngle(result);

}

void BarrierEnemy::Update()
{	
	Move();

	static const float ZERO = 0.0f;
	// hpがなくなったときに管理クラスから削除
	if (hp <= ZERO)
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
	// プレイヤーと衝突したら色変更　後程ボールと当たったときにも適用
	if (typeid(object) == typeid(Player))
	{
		modelObjects["main"].SetMulColor(MelLib::Color(100, 100, 100, 255));

	}

}

void BarrierEnemy::SetBallDir(const MelLib::Vector3& pos)
{
	// ボールの位置への方向ベクトルをとり変数にセット
	MelLib::Vector3 result = pos - GetPosition();

	ballDir = result.Normalize();
}

void BarrierEnemy::SetBarrier(std::shared_ptr<EnemyBarrier> barrier)
{
	pBarrier = barrier;
}
