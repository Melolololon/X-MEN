#include "Particle.h"


Particle::Particle()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
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
	// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
	model.get()->Draw();
}

