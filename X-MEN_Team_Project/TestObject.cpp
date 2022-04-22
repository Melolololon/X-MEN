#include "TestObject.h"

TestObject::TestObject(const MelLib::Vector3& pos)
{
	SetPosition(pos);
}

void TestObject::Update()
{
}

void TestObject::Draw()
{
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
