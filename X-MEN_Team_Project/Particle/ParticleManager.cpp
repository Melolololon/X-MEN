#include "ParticleManager.h"
#include<GameObjectManager.h>
#include<memory>

void ParticleManager::Initialize()
{
	count = 0;
	
	for (int i = 0; i < defeat_particle; i++)
	{
		normalParticle.emplace_back(std::make_shared<Particle>());
		//�ǉ�
		normalParticle[i].get()->SetColor(MelLib::Color(255,255,255,100));
	}
}

void ParticleManager::Update(MelLib::Vector3 target)
{
	for (int i = 0; i < normalParticle.size(); i++)
	{
		if (normalParticle[i].get()->GetLiveFlag())
		{
			if (normalParticle[i].get()->GetTIme() > 10)
			{
				normalParticle[i].get()->SetLiveFlag(false);
			}
		}
		normalParticle[i].get()->Update();
	}

	//���o����
	if (fire)
	{
		//���Ԃɉ����o��
		if (count < normalParticle.size())
		{
			if (!normalParticle[count]->GetLiveFlag()) {
				normalParticle[count]->SetLiveFlag(true); 
				//�T�C�Y�w��
				const float rScale = rand() % 40 + 10;
				normalParticle[count].get()->SetScale(MelLib::Vector3(rScale / 10.0f));
				//�|�W�V�����w��
				const float rPos = rand() % 20 - 10;
				normalParticle[count].get()->SetPosition(target + MelLib::Vector3(rPos / 10, 0, rPos / 10));
				//�A���O���w��
				const float rAngle = rand() % 360;
				normalParticle[count].get()->SetAngle(MelLib::Vector3(rAngle));
			}
		}
		else { count = 0; }

		count++;
	}
}

void ParticleManager::Draw()
{
	if (fire)
	{
		for (int i = 0; i < normalParticle.size(); i++)
		{
			if (normalParticle[i].get()->GetLiveFlag())
				normalParticle[i].get()->Draw();
		}
	}
}