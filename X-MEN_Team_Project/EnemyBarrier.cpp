#include "EnemyBarrier.h"

#include<Input.h>

#include"TestObject.h"

EnemyBarrier::EnemyBarrier()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	//初期値
	SetPosition(MelLib::Vector3(0, 0, 5));
	SetScale(MelLib::Vector3(5, 5, 1));
	nowHp = hp;
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
		if (hp < 0)
		{
			isOpen = false;
			nowHp = hp;
		}
	}
	modelObjects["main"].SetMulColor(MelLib::Color(255, 150, 150, 255));
}

void EnemyBarrier::Draw()
{
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

