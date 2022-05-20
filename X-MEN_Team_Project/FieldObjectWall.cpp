#include "FieldObjectWall.h"

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
