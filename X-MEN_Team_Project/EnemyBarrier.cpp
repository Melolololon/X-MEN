#include "EnemyBarrier.h"

#include<Input.h>

EnemyBarrier::EnemyBarrier()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	//初期値
	SetPosition(MelLib::Vector3(0, 0, 5));
	SetScale(MelLib::Vector3(5, 5, 1));
	nowHp = aaaa::HP;
	isOpen = false;

	// 当たり判定の作成(球)
	// EnemyBarrierの座標を取得し、それをセット
	//sphereDatas["main"].resize(1);
	//sphereDatas["main"][0].SetPosition(GetPosition());
	//sphereDatas["main"][0].SetRadius(0.5f);
}

void EnemyBarrier::Update()
{
	//展開中なら
	if (isOpen)
	{
		//耐久値が0になったら
		if (nowHp < 0)
		{
			isOpen = false;
		}
	}
	modelObjects["main"].SetMulColor(MelLib::Color(255, 150, 150, 255));
}

void EnemyBarrier::Draw()
{
	//展開中のみ描画
	if (isOpen)
	{
		// ModelObjectsに追加されているModelObjectをすべて描画
		AllDraw();
	}
}

void EnemyBarrier::Hit
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
}

void EnemyBarrier::SetBarrierPosition(MelLib::Vector3 positon, MelLib::Vector3 move)
{
	//展開中なら
	if (isOpen)
	{
		//体の向きをmoveから求める
		float direction = atan2f(move.x, move.z);
		//バリアを展開する座標
		MelLib::Vector3 barrierPosition;
		//引数のpositionを中心に半径radius分離れた座標を求める
		barrierPosition = MelLib::Vector3(positon.x + (sinf(direction) * aaaa::RADIUS), positon.y, positon.z + (cosf(direction) * aaaa::RADIUS));
		SetPosition(barrierPosition);
		//度数法→弧度法
		SetAngle(MelLib::Vector3(0.0f, direction * 57.32484076433121f, 0.0f));

	}
}

void EnemyBarrier::IsOpen()
{
	isOpen = true;
	nowHp = aaaa::HP;
}
