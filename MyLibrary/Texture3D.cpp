#include "Texture3D.h"
#include"CreateBuffer.h"

std::unordered_map < std::string, std::unique_ptr < MelLib::Texture3D >> MelLib::Texture3D::pTextures;

bool MelLib::Texture3D::Load(const std::vector<std::string>& path, const std::string& name)
{
    pTextures.emplace(name, std::make_unique<Texture3D>());
    return pTextures[name]->Load3DTexture(path);
}

bool MelLib::Texture3D::Load3DTexture(const std::vector<std::string>& texturePath)
{
	auto result = LoadTexture(texturePath);
	//“Ç‚Ýž‚ÝŽ¸”s
	if (!result)
	{
#ifdef _DEBUG

		OutputDebugString(L"Texture3D‚Ì“Ç‚Ýž‚Ý‚ÉŽ¸”s‚µ‚Ü‚µ‚½B\n");
#endif // _DEBUG

		return false;
	}

	GetImage(texturePath.size());

	
	CreateBuffer::GetInstance()->CreateTexture3DBuffer(metadata, image, &textureBuffer);


	return true;

}
