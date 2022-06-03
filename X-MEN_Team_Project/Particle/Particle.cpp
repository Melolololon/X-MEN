#include "Particle.h"


Particle::Particle()
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	model = std::make_unique<MelLib::ModelObject>();
	model.get()->Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	liveFlag = false;
	model.get()->SetPosition(MelLib::Vector3());

}

void Particle::Update()
{
	if (liveFlag)
	{
		time++;
	}
	else
	{
		time = 0;
	}
	model.get()->Update();
}

void Particle::Draw()
{
	// ModelObjectsに追加されているModelObjectをすべて描画
	model.get()->Draw();
}

