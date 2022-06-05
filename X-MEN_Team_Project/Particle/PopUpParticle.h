#pragma once
#include<memory>
#include<time.h>
#include"Particle.h"

namespace
{
	//���q�̌�
	int pop_particle = 10;
	//���q�̎���
	int pop_time = 5;
}
class PopUpParticle
{
private:
	//static PopUpParticle* instance;

	//�p�[�e�B�N���z��
	std::vector<std::shared_ptr<Particle>> particle;
	//�p�[�e�B�N�����o���ꏊ
	MelLib::Vector3 target;
	//���Ԃɕ��o����p�̃J�E���g
	int count;

public:
	//static PopUpParticle* GetInstance();
	// �����z�u�p
	void Initialize();
	void Update();
	void Draw();
	//Setter
	void SetFire(const bool b);
	void SetTarget(const MelLib::Vector3 t) { target = t; }

	//void Destroy();
};