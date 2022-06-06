#pragma once
#include<d3d12.h>
#include<DirectXTex.h>
#include<wrl.h>
#include<unordered_map>

#include"Vector.h"
#include"Color.h"
#include<memory>

//Textureにテクスチャバッファ持たせる?
//スプライトとかで同じテクスチャなのにバッファ毎回作るのもったいない
//

namespace MelLib
{
	//テクスチャ
	class Texture
	{
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		static UINT loadTextureNumber;
		static std::unordered_map<std::string, std::unique_ptr<Texture>>pTextures;

		//空いたヒープを使用するために使用
		static std::vector<UINT>eraseTextureNumber;

	private:
		std::string textureName;

		//テクスチャ番号(ヒープの添え字)
		UINT textureNumber = 0;

		// 
		std::vector< DirectX::ScratchImage> scratchImage;


	private:

		bool LoadSpriteTexture(const std::string& texturePath);

	protected:

		ComPtr<ID3D12Resource>textureBuffer;
		DirectX::TexMetadata metadata = {};
		std::vector<const DirectX::Image*>image;
	protected:
		/// <summary>
		/// DirectXTexの関数でメタデータを読み込むクラス。
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		bool LoadTexture(const std::vector<std::string>& path);
	
		/// <summary>
		/// スクラッチイメージからimageを取得
		/// </summary>
		/// <param name="textureNum"></param>
		void GetImage(const size_t textureNum);
	public:
		Texture() {}
		~Texture() {}



		static bool Load(const std::string& path, const std::string& name);
		static void Delete(const std::string& name);
		static Texture* Get(const std::string& name) { return pTextures[name].get(); }

		/// <summary>
		/// モデルのテクスチャを読み込みます。
		/// </summary>
		/// <param name="texturePath"></param>
		/// <returns></returns>
		bool LoadModelTexture(const std::string& texturePath);



#pragma region データ取得

		Vector2 GetTextureSize()const { return { static_cast<float>(metadata.width) , static_cast<float>(metadata.height) }; }
		std::string GetTextureName()const { return textureName; }
#pragma endregion

#pragma region 開発者用関数


		DirectX::TexMetadata GetMetadata()const { return metadata; }
		std::vector <const DirectX::Image*> GetImage()const { return image; }
		UINT GetTextureNumber() { return textureNumber; }
		ID3D12Resource* GetTextureBuffer() { return textureBuffer.Get(); }
#pragma endregion
	};

}
