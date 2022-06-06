#pragma once
#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include<wrl.h>
#include<tuple>
#include<map>

#include"Color.h"
#include"Values.h"
#include"Texture3D.h"
#include"PipelineState.h"

namespace MelLib
{

	// メモエリア
	//Materialを継承してパラメータを増やせるようにする
	//継承してもどうやってシェーダーに渡す?利用者にMapしてもらうしかない?
	//定数バッファを自由に作れるようにしたほうがいい?

	//マテリアル情報の定数バッファをマテリアルクラスに持たせる。
	//そうすれば、モデルに複数のマテリアル情報持たせる必要がない。
	//マテリアルの基底クラス作ってそこに定数バッファ持たせないといけない
	
	//ヒープはどこに持たせる?
	//テクスチャバッファ以外に持たせるものありそうだったらモデルに

	//ModelData生成したり読み込んだら、自動で初期マテリアル作る。

	//マテリアルのデータの構造体を作ったから、
	//マテリアル構造体にテンプレート使ったマテリアル情報持たせる?読み取れる?マップは読み取れなくてもできる?
	//配列にまとめられないから分ける
	//ライブラリで定義されてるものだけ内部で管理する仕様にする?
	//利用者が用意したマテリアルは自分で管理してもらう
	//テンプレート無理そうだからわける
	
	//マテリアルの型に合わせてパイプライン生成時に使用するシェーダー変える?
	//マテリアルにパイプライン持たせる?
	//パイプラインにレンダリングの情報(カリングとかレンダーターゲット数)とかあって、
	//マテリアルはあくまで見た目の情報を持つやつだからいらん。
	
	//マテリアルのクラス指定すると、そのマテリアルに適したシェーダーでパイプラインを作る機能はあってもいいかも
	//マテリアルごとに定数バッファとテクスチャバッファの数変わるから、ルートシグネチャ持たせちゃう? 
	//テンプレートのやつCreate呼べない。
	//継承して作ってもらう?

	//マテリアルにパイプライン持たせる
	//パイプラインはprotectedで持たせる。カリング処理の設定とかの構造体はprivateに持たせる。

	//とりあえず作るの進めよう



	//共通のマテリアル情報をまとめた基底クラス。
	class Material
	{
	public:
		
		enum class MaterialConstBufferType
		{
			MATERIAL_DATA,
			COLOR
		};

	private:

		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		static ID3D12Device* device;

		//マテリアルの定数バッファ
		ComPtr<ID3D12Resource>colorBuffer = nullptr;
		ComPtr<ID3D12Resource>materialBuffer = nullptr;

		//(マテリアルのカラー + AddColor + SubColor)*MulColor = 追加色
		Color color = Color(255, 255, 255, 255);
		
		//テクスチャ追加したらデストラクタとかにLoad関数で読み込んだテクスチャの開放処理を書くこと
		//書く場所は、デストラクタ、Set関数、Load関数の失敗時に入るif内の3箇所
		//テクスチャをポインタとSetTextureTypeを持ったtupleにすれば忘れないかも
		//テクスチャ
		static const UINT TEXTURE_HANDLE_NUM = 0;
		Texture* pTexture = nullptr;
		// シェーダー側で受け取るときにmapの方が指定しやすそうなので、mapにする
		std::map<std::string, Texture*>pTextures;
		
		static const UINT NORMAL_MAP_HANDLE_NUM = 1;
		Texture* pNormalMapTexture = nullptr;
		
		/*static const UINT TEXTURE_3D_HANDLE_NUM = 2;
		Texture3D* pTexture3D = nullptr;*/


		//テクスチャクラスに持たせるため、コメントアウト
		//ComPtr<ID3D12Resource>textureBuffer;
		
		//どこに持たせるのがベストかわからないため、保留
		//とりあえずマテリアルに持たせる(モデルごとに用意するのはもったいない)
		//UAVをテクスチャと同じヒープにまとめる必要が出てきたら、
		//コンピュートシェーダーを使うオブジェクトのみ専用のヒープ作る?
	    //ここに作ると複数のマテリアルのテクスチャをまとめられない
		//ファイルに複数モデルあってもレンダリングするタイミングが違うからまとめなくても問題ない
		ComPtr<ID3D12DescriptorHeap>textureHeap = nullptr;

		//テクスチャ未セット時にセットするテクスチャバッファ
		static ComPtr<ID3D12Resource> textureNoneTextureBuffer;

		// テクスチャ枚数の最大値
		static const unsigned int TEXTURE_MAX = 8;
	private:
		void MapColorBuffer(const Color& color);

		//メインのテクスチャをセットまたは読み込んだときに必ず呼び出す処理
		void SetOrLoadTextureProcess();
	protected:
		std::unique_ptr<PipelineState> pipelineState;
		DrawOption drawData;
		unsigned int textureNumMax = 0;

	protected:
		// エンジンとかだとマテリアル作った後にテクスチャ読み込んでセットできる。
		// それを再現するために、テクスチャを後からセットできるようにする。
		// しかし、先にヒープの数をセットしないといけないため、数を先に指定するようにする
		// あとこっちのほうが書き換えヶ所減るから都合いい

		/// <summary>
		/// マテリアル生成時の初期化処理を行います。
		/// </summary>
		/// <param name="mtlByte">構造体のバイト数</param>
		void CreateInitialize(const size_t& mtlByte, const unsigned int textureNum);

	
		void MapMaterialData(void** pData);
		void UnmapMaterialData();
	public:

		Material() {}
		virtual ~Material(){}
		

		static void Initialize(ID3D12Device* dev);
		
		virtual void Create(const DrawOption& drawData, const unsigned int textureNum){}

		ID3D12DescriptorHeap* GetPTextureHeap() { return textureHeap.Get(); }
		ID3D12Resource* GetPConstBuffer(const MaterialConstBufferType type)const;
		Texture* GetPTexture() { return pTexture; }
		void GetPTextures(std::vector<Texture*>& refTex) {};
		Texture* GetPNormalTexture() { return pNormalMapTexture; }
		//Texture3D* GetPTexture3D() { return pTexture3D; }
		PipelineState* GetPPipelineState() { return pipelineState.get(); }
	
		const std::map<std::string, Texture*>& GetRefPTextures()const { return pTextures; }

		void SetColor(const Color& color);
		/// <summary>
		/// テクスチャの登録、または交換を行います。
		/// </summary>
		/// <param name="pTex">テクスチャ名</param>
		/// <param name="name">登録名</param>
		/// <returns></returns>
		bool SetTexture(Texture* pTex,const std::string& name = "Default");
		//void SetTexture(const std::vector<Texture*>& pTex);
		//void SetTexture(const std::vector<Texture*>& pTex);
		void SetNormalMapTexture(Texture* pNormalMapTex);
		//void SetTexture3D(Texture3D* pTex);

	
		unsigned int GetTextureNum()const { return textureNumMax; }
		static unsigned int GetTextureNumMax() { return TEXTURE_MAX; }
	};


#pragma region ADSA

	struct ADSAMaterialData
	{


		//環境光
		Value4<float>ambient = 0.3f;
		//拡散反射光
		Value4<float>diffuse = 0.7f;
		//鏡面反射光
		Value4<float>specular = 0.0f;
		//アルファ値
		float alpha = 1.0f;

	};


	//Ambient、Diffuse、Supecular、モデルから読み取ったAlphaを所持したマテリアル
	class ADSAMaterial :public Material
	{
	private:
		//static std::unordered_map<std::string, std::unique_ptr<ADSAMaterial>>pMaterials;
		

		ADSAMaterialData materialData;
		void Map();
	public:
		ADSAMaterial(){}
		ADSAMaterial(ADSAMaterial& mtl);
		ADSAMaterial& operator=(ADSAMaterial& mtl);

		void Create(const DrawOption& drawData, const unsigned int textureNum)override;

#pragma region セット
		void SetMaterialData(const ADSAMaterialData& data);
#pragma endregion

#pragma region ゲット
		ADSAMaterialData GetMaterialData() { return materialData; }
#pragma endregion


	};
#pragma endregion

#pragma region PBR

	struct PBRMaterialData
	{
		//アルベドはMaterialに定義

		//金属度(0または1)
		float metalness = 1.0f;
		//鏡面反射光(float 0から1)
		float fSpecular = 0.5f;
		//粗さ
		float roughness = 0.0f;
	};

	//PBRマテリアル
	class PBRMaterial:public Material
	{
	private:
		PBRMaterialData materialData;

		void Map();
	public:
		PBRMaterial(){}
		PBRMaterial(PBRMaterial& mtl);
		PBRMaterial operator=(PBRMaterial& mtl);

		void Create(const DrawOption& drawData, const unsigned int textureNum)override;

#pragma region セット
		void SetMaterialData(const PBRMaterialData& data);
#pragma endregion

#pragma region ゲット
		PBRMaterialData GetMaterialData() { return materialData; }
#pragma endregion


	};

#pragma endregion


}
