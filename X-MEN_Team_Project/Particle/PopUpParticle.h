#pragma once
#include<memory>
#include<time.h>
#include"Particle.h"

namespace
{
	//���q�̌�
	int pop_particle = 10;
}
class PopUpParticle
{
private:
	//static PopUpParticle* instance;

	//�p�[�e�B�N���z��
	std::vector<std::shared_ptr<Particle>> particle;

	//���Ԃɕ��o����p�̃J�E���g
	int count;

public:
	//static PopUpParticle* GetInstance();
	// �����z�u�p
	void Initialize();
	void Update(MelLib::Vector3 target);
	void Draw();
	//Setter
	void SetFire(const bool b);

	//void Destroy();
};