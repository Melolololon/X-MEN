#include "FieldObjectWall.h"
#include "FieldObjectManager.h"

void FieldObjectWall::LoomingPosition()
{
	const float DIVIDE = 100;
	MelLib::Vector3 addVector = centerPos - GetPosition();
	addVector = addVector.Normalize() / DIVIDE;

	AddPosition(addVector);
}

void FieldObjectWall::LoomingScale()
{
	// 横幅を100分率した値
	const float MINUS_VALUE = FieldObjectWallInfo::TOP_BOTTOM_SIZE.x / 100;

	MelLib::Vector3 scale = GetScale();
	scale.x -= MINUS_VALUE / 100;

	SetScale(scale);
}

void FieldObjectWall::Looming()
{
	if (!FieldObjectManager::GetInstance()->IsLooming())return;

	LoomingPosition();
	LoomingScale();
}

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
	Looming();
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

void FieldObjectWall::SetCenterPosition(const MelLib::Vector3& setpos)
{
	centerPos = setpos;
}
