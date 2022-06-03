#include"FollowEnemy.h"

#include"../Player.h"
#include "../GameManager.h"

FollowEnemy::FollowEnemy()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット

	const float SCALE = 2;
	const float MODEL_SIZE = 2 * SCALE;
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
	sphereDatas["main"][0].SetRadius(MODEL_SIZE*0.5f);

	// 変数の初期化
	hp = FollowEnemyStatus::MAX_HP;
	playerDir = EnemyStatus::initPlayerDir;
	playerPos = EnemyStatus::initPlayerPos;
}

void FollowEnemy::Move()
{
	// 移動ベクトル
	MelLib::Vector3 moveVector;
	// 移動速度
	static const float MOVE_SPEED = 0.05f;

	moveVector = playerDir * MOVE_SPEED * GameManager::GetInstance()->GetGameTime();

	// 加算
	//0512一旦一定距離以上近づけさせないようにする
	AddPosition(moveVector);
}

void FollowEnemy::Update()
{
	if(!moveCancel)FollowToPlayer(FollowEnemyStatus::FOLLOW_SPEED);
	PushPosition();

	static const float ZERO = 0.0f;
	// hpがなくなったときに管理クラスから削除
	if (hp <= ZERO)
	{
		eraseManager = true;
	}

	modelObjects["main"].SetMulColor(MelLib::Color(255, 0, 255, 255));

}

void FollowEnemy::Draw()
{
	// ModelObjectsに追加されているModelObjectをすべて描画
	AllDraw();
}

void FollowEnemy::Hit(const GameObject& object, const MelLib::ShapeType3D shapeType, const std::string& shapeName, const MelLib::ShapeType3D hitObjShapeType, const std::string& hitShapeName)
{
	Enemy::Hit(object, shapeType, shapeName, hitObjShapeType, hitShapeName);
	// プレイヤーと衝突したら色変更　後程ボールと当たったときにも適用
	if (typeid(object) == typeid(Player))
	{
		modelObjects["main"].SetMulColor(MelLib::Color(100, 100, 100, 255));
	}
}
