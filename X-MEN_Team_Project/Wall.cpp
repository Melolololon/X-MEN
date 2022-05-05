#include "Wall.h"

Wall::Wall()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	modelObjects["main"].SetScale({ 20,1,1 });;
	// 初期位置を0,0,5に
	SetPosition(MelLib::Vector3(0, 0, 20));

	// 当たり判定の作成(球)
	// Playerの座標を取得し、それをセット
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
