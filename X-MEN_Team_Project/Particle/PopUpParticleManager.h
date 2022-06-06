#pragma once
#include<memory>
#include<time.h>
#include"PopUpParticle.h"

namespace
{
	//�p�[�e�B�N���̌�
	int pop_particle_count = 3;
}
class PopUpParticleManager
{
private:
	static PopUpParticleManager* instance;

	//�p�[�e�B�N���z��
	std::vector<std::shared_ptr<PopUpParticle>> particle;

public:
	static PopUpParticleManager* GetInstance();
	// �����z�u�p
	void Initialize();
	void Update();
	void Draw();
	//Setter
	void SetFire(MelLib::Vector3 target);

	void Destroy();
};