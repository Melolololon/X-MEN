#include "PopUpParticleManager.h"
#include<GameObjectManager.h>
#include<memory>
#include<Easing.h>

// ê√ìIÇ»é¿ëÃ
PopUpParticleManager* PopUpParticleManager::instance;

PopUpParticleManager* PopUpParticleManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new PopUpParticleManager();
	}

	return instance;
}

void PopUpParticleManager::Initialize()
{

	for (int i = 0; i < pop_particle_count; i++)
	{
		//í«â¡
		particle.emplace_back(std::make_shared<PopUpParticle>());
		particle[i].get()->Initialize();
	}
}

void PopUpParticleManager::Update()
{
	for (int i = 0; i < particle.size(); i++)
	{
		particle[i].get()->Update();
	}
}

void PopUpParticleManager::Draw()
{
	for (int i = 0; i < particle.size(); i++)
	{
		particle[i].get()->Draw();
	}
}

void PopUpParticleManager::SetFire(MelLib::Vector3 target)
{
	for (int i = 0; i <particle.size(); i++)
	{
		if (!particle[i].get()->GetLiveFlag())
		{
			particle[i].get()->SetFire(true);
			particle[i].get()->SetTarget(target);
			break;
		}
	}
}

void PopUpParticleManager::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}

}

