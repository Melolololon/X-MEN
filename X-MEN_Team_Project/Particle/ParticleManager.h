#pragma once
#include<memory>
#include<time.h>
#include"Particle.h"

namespace
{
	//���q�̌�
	int defeat_particle = 10;
}
class ParticleManager
{
private:
	//static ParticleManager* instance;

	//�p�[�e�B�N���z��
	std::vector<std::shared_ptr<Particle>> normalParticle;
	//���o�t���O
	bool fire;
	//���Ԃɕ��o����p�̃J�E���g
	int count;

public:
	//static ParticleManager* GetInstance();
	// �����z�u�p
	void Initialize();
	void Update(MelLib::Vector3 target);
	void Draw();
	//Setter
	void SetFireFlag(const bool b) { fire = b; }

	//void Destroy();
};