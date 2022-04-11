#include "Emitter2D.h"

MelLib::Emitter2D::Emitter2D(const Particle2D* const pParticle, unsigned int particleNum)
{
	
	
	particles.resize(particleNum);

}

void MelLib::Emitter2D::Update()
{
	for(const auto& particle : particles)
	{
	}
}

void MelLib::Emitter2D::Draw()
{
	for (const auto& particle : particles)
	{
	}
}
