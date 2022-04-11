#include "Particle3D.h"



MelLib::Particle3D::Particle3D(const Color& color)
{
	particleSprite.Create(color);
	//particleSprite.SetColor(color);
}

MelLib::Particle3D::Particle3D(Texture* pTexture)
{
	particleSprite.Create(pTexture);
	//particleSprite.SetTexture(pTexture);
}

MelLib::Particle3D::Particle3D(const Particle3D& particle):
	particleSprite(particle.particleSprite)
{
}

MelLib::Particle3D::Particle3D(const Particle3D& particle, const Vector3& pos) :
	particleSprite(particle.particleSprite)
{
	startPosition = pos;
	particleSprite.SetPosition(pos);
}

void MelLib::Particle3D::Initialize()
{
}

void MelLib::Particle3D::Update()
{
}

void MelLib::Particle3D::Draw()
{
	particleSprite.Draw();
}
