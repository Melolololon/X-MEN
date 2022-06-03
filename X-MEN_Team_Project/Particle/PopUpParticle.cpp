#include "PopUpParticle.h"
#include<GameObjectManager.h>
#include<memory>
#include<Easing.h>

void PopUpParticle::Initialize()
{
	count = 0;

	for (int i = 0; i < pop_particle; i++)
	{
		//�ǉ�
		particle.emplace_back(std::make_shared<Particle>());

		particle[i].get()->SetColor(MelLib::Color(255, 255, 255, 255));
	}
}

void PopUpParticle::Update(MelLib::Vector3 target)
{
	const int time = particle[0].get()->GetTIme();
	//�o�߂����������
	if (time > 10)
	{
		for (auto& p : particle)
		{
			p.get()->SetLiveFlag(false);
		}
	}
	for (int i = 0; i < particle.size(); i++)
	{
		if (particle[i].get()->GetLiveFlag())
		{
			//�~��Ɋg�U
			const float b = 360 / particle.size() * i;
			//��΂��p�x
			const MelLib::Vector3 dir = MelLib::Vector3(sin(b), 0, cos(b));
			particle[i].get()->SetPosition(target + dir * time * 2);
			//�x���@���ʓx�@
			particle[i].get()->SetAngle(MelLib::Vector3(0, atan2f(dir.x, dir.z) * 57.32484076433121f, 0));
			particle[i].get()->SetScale(MelLib::Vector3(1, 1, MelLib::Easing(6.0f, 0.0f, time * 10).EaseIn()));

		}
		particle[i].get()->Update();
	}
}

void PopUpParticle::Draw()
{
		for (int i = 0; i < particle.size(); i++)
		{
			if (particle[i].get()->GetLiveFlag())
			{
				particle[i].get()->Draw();
			}
		}
}

void PopUpParticle::SetFire(const bool b)
{
	for (auto& p : particle)
	{
		p.get()->SetLiveFlag(true);
	}
}
