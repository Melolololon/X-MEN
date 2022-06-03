#pragma once
#include<memory>
#include<time.h>
#include"Particle.h"

namespace
{
	//粒子の個数
	int defeat_particle = 10;
}
class ParticleManager
{
private:
	//static ParticleManager* instance;

	//パーティクル配列
	std::vector<std::shared_ptr<Particle>> normalParticle;
	//噴出フラグ
	bool fire;
	//順番に放出する用のカウント
	int count;

public:
	//static ParticleManager* GetInstance();
	// 初期配置用
	void Initialize();
	void Update(MelLib::Vector3 target);
	void Draw();
	//Setter
	void SetFireFlag(const bool b) { fire = b; }

	//void Destroy();
};