#include "PopUpParticle.h"
#include<GameObjectManager.h>
#include<memory>
#include<Easing.h>

void PopUpParticle::Initialize()
{
	target = MelLib::Vector3();
	count = 0;

	for (int i = 0; i < pop_particle; i++)
	{
		//追加
		particle.emplace_back(std::make_shared<Particle>());

		particle[i].get()->SetColor(MelLib::Color(255, 255, 255, 255));
	}
}

void PopUpParticle::Update()
{
	const int time = particle[0].get()->GetTIme();
	//経過したら消える
	if (time > pop_time)
	{
		for (auto& p : particle)
		{
			p.get()->SetPosition(target);
			p.get()->SetLiveFlag(false);
		}
	}
	for (int i = 0; i < particle.size(); i++)
	{
		if (particle[i].get()->GetLiveFlag())
		{
			//円状に拡散
			const float b = 360 / particle.size() * i;
			//飛ばす角度
			const MelLib::Vector3 dir = MelLib::Vector3(sin(b), 0, cos(b));
			particle[i].get()->SetPosition(target + dir * (time * 2));
			//度数法→弧度法
			particle[i].get()->SetAngle(MelLib::Vector3(0, atan2f(dir.x, dir.z) * 57.32484076433121f, 0));
			particle[i].get()->SetScale(MelLib::Vector3(1.0f, 1.0f, MelLib::Easing(3.0f, 0.0f, time * (100.0f / pop_time)).EaseIn()));

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
