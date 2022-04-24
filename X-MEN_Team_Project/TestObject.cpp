#include "TestObject.h"

TestObject::TestObject(const MelLib::Vector3& pos)
{
	SetPosition(pos);

	// MelLib;;ModelObject�̔z��
    // �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);
}

void TestObject::Update()
{
}

void TestObject::Draw()
{
	AllDraw();
}

void TestObject::Hit
(
	const GameObject& object, 
	const MelLib::ShapeType3D shapeType, 
	const std::string& shapeName, 
	const MelLib::ShapeType3D hitObjShapeType,
	const std::string& hitShapeName
)
{
}
