#include "NormalBarrier.h"

#include<Input.h>

#include"TestObject.h"


NormalBarrier::NormalBarrier()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	//初期値
	SetPosition(MelLib::Vector3(0, 0, 5));
	SetScale(MelLib::Vector3(5, 5, 5));
	time = 0;
	isOpen = false;

	// 当たり判定の作成(球)
	// NormalBarrierの座標を取得し、それをセット
	//sphereDatas["main"].resize(1);
	//sphereDatas["main"][0].SetPosition(GetPosition());
	//sphereDatas["main"][0].SetRadius(0.5f);
}

void NormalBarrier::Update()
{
	//展開中なら
	if (isOpen)
	{
		//展開時間カウント
		time++;
		//指定した展開の時間経過したら
		if (time > openTime)
		{
			isOpen = false;
			time = 0;
		}
	}
	modelObjects["main"].SetMulColor(MelLib::Color(150, 150, 255, 255));
}

void NormalBarrier::Draw()
{
	// ModelObjectsに追加されているModelObjectをすべて描画
	AllDraw();
}

void NormalBarrier::Hit
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

void NormalBarrier::SetIsOpen(bool flag)
{
	isOpen = flag;
}
