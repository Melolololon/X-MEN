#include "Texture.h"
#include"CreateBuffer.h"
#include"Sprite.h"

using namespace MelLib;
std::unordered_map<std::string, std::unique_ptr<Texture>>Texture::pTextures;
std::vector<UINT>Texture::eraseTextureNumber;
UINT Texture::loadTextureNumber = 1;


bool Texture::LoadTexture(const std::vector<std::string>& path)
{
	size_t pathStrNum = path.size();


	scratchImage.resize(pathStrNum);
	image.resize(pathStrNum);
	
	for (size_t i = 0; i < pathStrNum; i++)
	{
		std::wstring texturePathW;
		texturePathW.resize(path[i].size());
		MultiByteToWideChar
		(
			CP_ACP,
			0,
			path[i].c_str(),
			-1,
			texturePathW.data(),
			texturePathW.size()
		);

		
		auto result = LoadFromWICFile
		(
			texturePathW.c_str(),
			DirectX::WIC_FLAGS_NONE,
			&metadata,
			scratchImage[i]
		); 
		
		if (result != S_OK)
		{
#ifdef _DEBUG
			OutputDebugString(texturePathW.c_str());
			OutputDebugString(L"‚Ì“Ç‚Ýž‚Ý‚ÉŽ¸”s‚µ‚Ü‚µ‚½B\n");
#endif // _DEBUG
			return false;
		}
	}


	return true;
}

void MelLib::Texture::GetImage(const size_t textureNum)
{
	for(int i = 0; i < textureNum;i++)
	{
		image[i] = scratchImage[i].GetImage(0, 0, 0);
	}
}

bool Texture::Load(const std::string& path, const std::string& name)
{
	pTextures.emplace(name, std::make_unique<Texture>());
	return pTextures[name]->LoadSpriteTexture(path);

}

void Texture::Delete(const std::string& name)
{
	eraseTextureNumber.push_back(pTextures[name]->GetTextureNumber());
	pTextures.erase(name);
}

bool Texture::LoadModelTexture(const std::string& texturePath)
{
	std::vector<std::string>path(1, texturePath);
	auto result = LoadTexture(path);
	//“Ç‚Ýž‚ÝŽ¸”s
	if (!result)
	{
#ifdef _DEBUG
		OutputDebugString(L"ƒ‚ƒfƒ‹‚ÌƒeƒNƒXƒ`ƒƒ‚Ì“Ç‚Ýž‚Ý‚ÉŽ¸”s‚µ‚Ü‚µ‚½B\n");
		return false;
#endif // _DEBUG
	}

	GetImage(1);
	CreateBuffer::GetInstance()->CreateTextureBuffer(metadata, image[0], &textureBuffer);

	return true;
}


bool Texture::LoadSpriteTexture(const std::string& texturePath)
{
	std::vector<std::string>path(1, texturePath);
	auto result = LoadTexture(path);
	//“Ç‚Ýž‚ÝŽ¸”s
	if (!result)
	{
		return false;
	}

	GetImage(1);

	if (eraseTextureNumber.size() == 0)
	{
		textureNumber = loadTextureNumber;
		loadTextureNumber++;
	}
	else
	{
		textureNumber = eraseTextureNumber[eraseTextureNumber.size() - 1];
		eraseTextureNumber.pop_back();
	}

	CreateBuffer::GetInstance()->CreateTextureBuffer(metadata, image[0], &textureBuffer);

	Sprite::CreateTextureBuffer(this);

	return true;
}
