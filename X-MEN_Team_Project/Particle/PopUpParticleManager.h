#pragma once
#include<memory>
#include<time.h>
#include"PopUpParticle.h"

namespace
{
	//パーティクルの個数
	int pop_particle_count = 3;
}
class PopUpParticleManager
{
private:
	static PopUpParticleManager* instance;

	//パーティクル配列
	std::vector<std::shared_ptr<PopUpParticle>> particle;

public:
	static PopUpParticleManager* GetInstance();
	// 初期配置用
	void Initialize();
	void Update();
	void Draw();
	//Setter
	void SetFire(MelLib::Vector3 target);

	void Destroy();
};