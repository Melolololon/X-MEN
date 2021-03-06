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
	//読み込み失敗
	if (!result)
	{
#ifdef _DEBUG

		OutputDebugString(L"Texture3Dの読み込みに失敗しました。\n");
#endif // _DEBUG

		return false;
	}

	GetImage(texturePath.size());

	
	CreateBuffer::GetInstance()->CreateTexture3DBuffer(metadata, image, &textureBuffer);


	return true;

}
