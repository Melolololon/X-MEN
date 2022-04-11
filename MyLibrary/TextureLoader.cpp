#include "TextureLoader.h"


using namespace MelLib;
//
//BMPData TextureLoader::LoadBMP(const char* texturepath)
//{
//	FILE* texture;
//	fopen_s(&texture, texturepath, "rb");
//	//パスでファイル形式を確認する
//	int size = strlen(texturepath);
//
//	if (texturepath[size - 1] != 'p')
//	{
//		//戻り値をvoidにして、構造体は引数で受け取るようにする
//		BMPData null;
//		return null;
//	}
//
//	BMPData bmpData;
//	fread(&bmpData, sizeof(bmpData) - 16, 1, texture);
//
//	if (bmpData.bit == 32)
//	{
//		unsigned char sig[84];
//		fread(&sig, sizeof(sig), 1, texture);
//	}
//
//	bmpData.rgbaData.resize(bmpData.height * bmpData.width);
//	fread(bmpData.rgbaData.data(), bmpData.rgbaData.size() * 4, 1, texture);
//
//	std::reverse(bmpData.rgbaData.begin(), bmpData.rgbaData.end());
//	for (int i = 0; i < (int)bmpData.height; i++)
//	{
//		std::reverse(bmpData.rgbaData.begin() + bmpData.width * i, bmpData.rgbaData.begin() + bmpData.width * i + bmpData.width);
//	}
//
//	fclose(texture);
//
//
//	return bmpData;
//	
//}

TextureLoader* MelLib::TextureLoader::GetInstance()
{
	static TextureLoader t;
	return &t;
}

bool MelLib::TextureLoader::LoadTexture(const std::vector<std::string>& path, std::vector<DirectX::TexMetadata>& metadata, std::vector<DirectX::ScratchImage>& scratchImage)
{
	return false;
}
