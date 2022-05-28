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
	// ������100���������l
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
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// �����ʒu��0,0,5
	SetPosition(MelLib::Vector3(0, 0, 0));

	// �����蔻��̍쐬(BOX)

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
