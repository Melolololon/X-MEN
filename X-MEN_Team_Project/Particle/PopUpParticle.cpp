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
		object.emplace_back(std::make_shared<Particle>());

		object[i].get()->SetColor(MelLib::Color(255, 255, 255, 255));
	}
}

void PopUpParticle::Update()
{
	const int time = object[0].get()->GetTIme();

	static float division = (628.0f / object.size()) / 100;

	//経過したら消える
	if (time > pop_time)
	{
		for (auto& p : object)
		{
			p.get()->SetPosition(target);
			p.get()->SetLiveFlag(false);
		}
	}
	for (int i = 0; i < object.size(); i++)
	{
		if (object[i].get()->GetLiveFlag())
		{
			float angle = division * i;
			object[i].get()->SetPosition(target + MelLib::Vector3(sinf(angle)* time,0.0f, cosf(angle) * time));
			//オブジェクトの角度
			object[i].get()->SetAngle(MelLib::Vector3(0, angle, 0));
			//サイズ縮小
			object[i].get()->SetScale(MelLib::Vector3(1.0f, 1.0f, MelLib::Easing(3.0f, 0.0f, time * (100.0f / pop_time)).EaseIn()));

		}
		object[i].get()->Update();
	}
}

void PopUpParticle::Draw()
{
		for (int i = 0; i < object.size(); i++)
		{
			if (object[i].get()->GetLiveFlag())
			{
				object[i].get()->Draw();
			}
		}
}

void PopUpParticle::SetFire(const bool b)
{
	for (auto& p : object)
	{
		p.get()->SetLiveFlag(true);
	}
}

bool PopUpParticle::GetLiveFlag() const
{
	return object[0].get()->GetLiveFlag();
}
