#pragma once
#include<vector>
#include<string>
#include"DirectXTex.h"
#include"DirectXStruct.h"

namespace MelLib 
{
	class TextureLoader
	{
	private:
		TextureLoader(){}
		~TextureLoader(){}

	public:
		//static BMPData LoadBMP(const char* texturepath);

		TextureLoader(TextureLoader& t) = delete;
		TextureLoader& operator=(TextureLoader& t) = delete;
		static TextureLoader* GetInstance();

		bool LoadTexture
		(
			const std::vector<std::string>& path,
			std::vector<DirectX::TexMetadata>& metadata, 
			std::vector<DirectX::ScratchImage>& scratchImage
		);
	};
}
