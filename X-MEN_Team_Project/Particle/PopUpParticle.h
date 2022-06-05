#pragma once
#include<memory>
#include<time.h>
#include"Particle.h"

namespace
{
	//粒子の個数
	int pop_particle = 10;
	//粒子の寿命
	int pop_time = 5;
}
class PopUpParticle
{
private:
	//static PopUpParticle* instance;

	//パーティクル配列
	std::vector<std::shared_ptr<Particle>> particle;
	//パーティクルを出す場所
	MelLib::Vector3 target;
	//順番に放出する用のカウント
	int count;

public:
	//static PopUpParticle* GetInstance();
	// 初期配置用
	void Initialize();
	void Update();
	void Draw();
	//Setter
	void SetFire(const bool b);
	void SetTarget(const MelLib::Vector3 t) { target = t; }

	//void Destroy();
};