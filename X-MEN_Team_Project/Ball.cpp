#include "Ball.h"
#include "FieldObjectManager.h"
#include "FieldObjectWall.h"
#include <Random.h>
#include <LibMath.h>


void Ball::Move()
{
	//位置更新
	SetPosition(GetPosition() + velocity * speed);

	//スピード少し減らす
	speed -= 0.001f;
	if (speed < 0) { speed = 0; }
}

Ball::Ball()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	//色セット (暫定で黄色)
	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 64, 255));

	// 当たり判定の作成(球)
	// Playerの座標を取得し、それをセット
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);

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

		//停止したら自分の手元に戻る
		if (speed <= 0) {
			isThrowed = false;
		}
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

	//壁との反射
	if (typeid(object) == typeid(FieldObjectWall))
	{
		//反射ベクトルを計算
		Vector3 otherNormal = GetSphereCalcResult().GetBoxHitSurfaceNormal();
		Vector3 reflectVel = (velocity - 2.0f * velocity.Dot(otherNormal) * otherNormal);

		//反映
		velocity = reflectVel;
	}
}

void Ball::ThrowBall(const Vector3& initPos)
{
	//XZ平面のランダムな角度で射出
	float rand = MelLib::Random::GetRandomFloatNumberRangeSelect(0, 2 * MelLib::LibMath::GetFloatPI(), 2);
	float initX = sin(rand);
	float initZ = cos(rand);

	//スピードを初期値に
	speed = INIT_SPEED;

	//位置、方向セット
	SetBallPos(initPos);
	velocity = { initX, 0, initZ };

	//射出フラグを有効に
	isThrowed = true;
}

void Ball::SetBallPos(const Vector3& pos)
{
	//オブジェクトに反映
	SetPosition(pos);
}
