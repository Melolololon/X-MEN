#include "FieldObjectWall.h"

FieldObjectWall::FieldObjectWall()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// �����ʒu��0,0,5
	SetPosition(MelLib::Vector3(0, 0, 0));

	// �����蔻��̍쐬(BOX)

	boxDatas["main"].resize(1);
}

void FieldObjectWall::Update()
{
	SetPosition(GetPosition());
}

void FieldObjectWall::Draw()
{
	AllDraw();
}
