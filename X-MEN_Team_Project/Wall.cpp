#include "Wall.h"

Wall::Wall()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	modelObjects["main"].SetScale({ 20,1,1 });;
	// �����ʒu��0,0,5��
	SetPosition(MelLib::Vector3(0, 0, 20));

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
	boxDatas["main"].resize(1);
	boxDatas["main"][0].SetPosition(GetPosition());
	boxDatas["main"][0].SetSize(testSize);

	//sphereDatas["main"].resize(1);
	//sphereDatas["main"][0].SetPosition(GetPosition());
	//sphereDatas["main"][0].SetRadius(0.5f);

}

void Wall::Update()
{
}

void Wall::Draw()
{
	AllDraw();
}

void Wall::Hit(const GameObject& object, const MelLib::ShapeType3D shapeType, const std::string& shapeName, const MelLib::ShapeType3D hitObjShapeType, const std::string& hitShapeName)
{

}
