#include "FieldObjectWall.h"

FieldObjectWall::FieldObjectWall()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// 初期位置を0,0,5
	SetPosition(MelLib::Vector3(0, 0, 0));

	// 当たり判定の作成(BOX)

	obbDatas["main"].resize(1);
}

void FieldObjectWall::Update()
{
	SetPosition(GetPosition());
	SetScale(GetScale());
	SetAngle(GetAngle());

	obbDatas["main"][0].SetPosition(GetPosition());
	obbDatas["main"][0].SetSize(GetScale());
}

void FieldObjectWall::Draw()
{
	AllDraw();
}
