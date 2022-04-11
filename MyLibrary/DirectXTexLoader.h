#pragma once
#include<DirectXTex.h>
#include"DirectXStruct.h"
namespace MelLib
{
	class DirectXTexLoader
	{
	public:

		static const DirectX::Image* LoadTexture(const wchar_t* path, DirectX::TexMetadata* meta, DirectX::ScratchImage* scratchImage);

	};
}
