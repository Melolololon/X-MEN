#pragma once
#include<vector>
#include<d3d12.h>
#include<DirectXMath.h>
#include<wrl.h>
#include<string>
using namespace Microsoft::WRL;

namespace MelLib
{

	//各データにアクセスするためのもの
#pragma region バッファデータキー

//これDirectXStructに移して、モデルの頂点バッファ作るところでこのenumセットしてもいいかも
//これパイプラインセットするときに設定するようにする?
//その場合、ヒープの構造体も作ったほうがいい?
//これ新しいヘッダーファイル作ってそこに書く?(DirectXに直接関わらないし)
	enum VertexType
	{
		VERTEX_TYPE_NONE,//未設定(まだ頂点バッファを生成していない)
		VERTEX_TYPE_NORMAL,//座標、uv、法線のみ
		VERTEX_TYPE_OBJ_ANIMATION,//座標、uv、法線、ボーン番号
		VERTEX_TYPE_USER_VERTEX,//利用者の自作データ

	};

	//モデルのデータを所持します。
	struct ModelDataAndKey
	{
		std::string key;//モデルデータを呼び出すための文字列
		VertexType type;//頂点構造体の種類
	};


#pragma endregion

	//DirectX12で描画するために使用する構造体
#pragma region シェーダーに送る情報
//頂点バッファで送る情報
//Vertex.h作ってそっちに実装する
#pragma region 頂点構造体




	struct ObjAnimationVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		UINT boneNumber;
	};

	struct Vertex
	{
		/*Vertex();
		Vertex(const Vertex& vertex);
		Vertex(const ObjAnimationVertex& objVertex);
		~Vertex();*/
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;//ポリゴンのどこら辺かをあらわすもの　ポリゴン上の座標
		DirectX::XMFLOAT3 normal;
	};

	struct FbxVertex
	{
		static const int MAX_BONE_INDICES = 4;
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		UINT boneIndex[MAX_BONE_INDICES] = { 63,0,0,0 };
		float boneWeight[MAX_BONE_INDICES] = { 1,0,0,0 };
	};

	struct SpriteVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};


	struct PointVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 scale;
		//DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT4 color;

	};



#pragma region PMD


	struct PMDVertex
	{
#pragma pack(2)

		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		unsigned short bone[2];
		unsigned char boneWeight;
		unsigned char edgeFlg;

#pragma pack()
	};

	struct PMDHeader
	{
		float version;
		char modelName[20];
		char comment[256];
	};
#pragma endregion

#pragma endregion


#pragma region 定数構造体

	struct ModelConstData
	{
		DirectX::XMFLOAT3 position = { 0,0,0 };
		DirectX::XMFLOAT3 scale = { 1,1,1 };
		DirectX::XMFLOAT3 angle = { 0,0,0 };
		DirectX::XMFLOAT4 addColor = { 0,0,0,0 };
		DirectX::XMFLOAT4 subColor = { 0,0,0,0 };
		DirectX::XMFLOAT4 mulColor = { 1,1,1,1 };
		float pushPolygonNum = 0.0f;

	};

	struct SpriteConstData
	{
		DirectX::XMFLOAT3 position = { 0,0,0 };
		DirectX::XMFLOAT2 scale = { 1,1 };
		DirectX::XMFLOAT3 angle = { 0,0,0 };
		DirectX::XMFLOAT4 color = { 0,0,0,0 };//テクスチャ貼らない場合の色(テクスチャ貼り付け時は常に0)
		DirectX::XMFLOAT4 addColor = { 0,0,0,0 };
		DirectX::XMFLOAT4 subColor = { 0,0,0,0 };
		DirectX::XMFLOAT4 mulColor = { 1,1,1,1 };
	};

	//表示してるモデル1つ1つが違う値の構造体
	struct BoneData
	{
		//0,0,0から、どのくらい動かすかを表したfloat3
		DirectX::XMFLOAT3 moveVector = { 0,0,0 };
		DirectX::XMFLOAT3 scale = { 1,1,1 };
		DirectX::XMFLOAT3 angle = { 0,0,0 };
	};

	//ボーンの親をセットするときにセットする値
	struct ParentBoneData
	{
		int parentBoneNum = -1;//-1を未セットにするため、int型
		DirectX::XMFLOAT3 angleImpact = { 1,1,1 };
		DirectX::XMFLOAT3 scaleImpact = { 1,1,1 };
		DirectX::XMFLOAT3 moveVectorImpact = { 1,1,1 };
	};

	struct ModelConstBufferData
	{
		DirectX::XMFLOAT4 lightColor[20];
		DirectX::XMFLOAT4 light[20];
		DirectX::XMFLOAT4 cameraPos;
		DirectX::XMMATRIX mat;
		DirectX::XMMATRIX normalMat;
		DirectX::XMMATRIX worldMat;
		DirectX::XMFLOAT4 mulColor;
		DirectX::XMFLOAT4 addColor;
		DirectX::XMFLOAT4 subColor;
		float tex3DStart;
		float tex3DEnd;
		float ex;
	};

	struct CommonConstData
	{
		//float4にするかパック詰めないと送れない(float4以外の変数と次の変数の間に入り込む)からfloat4にしてる
		DirectX::XMFLOAT4 lightColor;
		DirectX::XMFLOAT4 light;
		DirectX::XMFLOAT4 cameraPos;
		DirectX::XMMATRIX lightMat;//ライト用行列
	};

	struct BoneConstBufferData
	{
		DirectX::XMMATRIX boneMat[64];
	};


	struct SpriteConstBufferData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 mulColor;
		DirectX::XMFLOAT4 addColor;
		DirectX::XMFLOAT4 subColor;
	};

	struct PointConstBufferData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMMATRIX billboardMat;
	};


#pragma region OBJ


	////データ格納用
	//struct MaterialData
	//{
	//	DirectX::XMFLOAT3 ambient;
	//	DirectX::XMFLOAT3 diffuse;
	//	DirectX::XMFLOAT3 specular;
	//	float alpha;


	//	std::string materialName;
	//	std::string textureName;

	//	MaterialData()
	//	{
	//		ambient = { 0.3f,0.3f,0.3f };
	//		diffuse = { 0.7f,0.7f,0.7f };
	//		specular = { 0.0f,0.0f,0.0f };
	//		alpha = 1.0f;
	//	}
	//};


	////データMap用
	//struct MaterialConstBufferData
	//{
	//	DirectX::XMFLOAT3 ambient;
	//	float pad1;
	//	DirectX::XMFLOAT3 diffuse;
	//	float pad2;
	//	DirectX::XMFLOAT3 specular;
	//	float alpha;


	//};


	//struct PbrMaterial
	//{
	//	//アルベド
	//	DirectX::XMFLOAT3 baseColor = { 1,1,1 };
	//	//金属度(0または1)
	//	float metalness = 0.0f;
	//	//鏡面反射光(float 0から1)
	//	float fSpecular = 0.5f;
	//	//粗さ
	//	float roughness = 0.0f;
	//};

	//struct PbrMaterialConstBufferData
	//{
	//	DirectX::XMFLOAT3 baseColor;
	//	float metalness;
	//	float fSpecular;
	//	float roughness;
	//	float pad[2];
	//};

#pragma endregion



#pragma endregion

#pragma region テクスチャ類
	//テクスチャバッファで送る情報
	struct RGBA
	{
		unsigned char r, g, b, a;
	};
#pragma region 自力読み込み

#pragma pack(2)
	//BMPを読み込み時に使用
	struct BMPData
	{
		char fileType[2];//ファイル形式
		long length;//ファイルサイズ
		short areaOne;//予備領域1
		short areaTwo;//予備領域2
		unsigned long offset;//ファイル先頭から画像データまでのオフセット

		unsigned long size;//サイズ
		unsigned long width;//横
		unsigned long height;//縦
		short plane;//プレーン数
		short bit;//色ビット数
		unsigned long compression;//圧縮形式
		unsigned long imageSize;//画像データサイズ
		long meterX;//水平解像度
		long meterY;//垂直解像度
		unsigned long parret;//格納パレット
		unsigned long important;//重要色素
		std::vector<RGBA> rgbaData;
	};
#pragma pack()


#pragma endregion

#pragma region スプライトフォント
	struct SpriteFontData
	{
		int widthLineNum;//横一行の文字数
		int heightLineNum;//横一行の文字数
		int fontSizeX;
		int fontSizeY;
	};

#pragma endregion


#pragma endregion

#pragma region バッファ類

	struct WorldMatData
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 angle;
		DirectX::XMFLOAT3 scale;
	};

#pragma region 汎用バッファセット

	//バッファー、Mapするための変数、ビューのセット
#pragma region 頂点


	struct VertexBufferSet
	{
		ComPtr<ID3D12Resource> vertexBuffer;
		Vertex* vertexMap;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		std::string materialName;
	};

	struct PMDVertexBufferSet
	{
		ComPtr<ID3D12Resource> vertexBuffer;
		PMDVertex* vertexMap;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	};
#pragma endregion


	struct IndexBufferSet
	{
		ComPtr<ID3D12Resource> indexBuffer;
		unsigned short* indexMap;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
	};

	struct ConstBufferSet
	{
		//書き換え多くなるから仮に配列使ってない

		//obj内のモデルごとに変更できるようにするために配列
		std::vector<ComPtr<ID3D12Resource>> constBuffer;
	};

	struct TextureBufferSet
	{
		//書き換え多くなるから仮に配列使ってない

		//.obj内のオブジェクトごとにテクスチャ違う可能性あるから配列
		std::vector<ComPtr<ID3D12Resource>> textureBuffer;
	};


#pragma endregion

#pragma region ユーザーバッファ
	//
	////これいらん?
	//struct UserVertexBufferSet
	//{
	//	ComPtr<ID3D12Resource> vertexBuffer;
	//	void* vertexMap;
	//	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	//};
	//
	//
	//struct UserConstBufferSet
	//{
	//	ComPtr<ID3D12Resource> constBuffer;
	//	void* constMaps;
	//};

#pragma endregion


	struct DepthBufferSet
	{
		ComPtr<ID3D12Resource> depthBuffer;
	};

	//
	//struct Object3DVertData
	//{
	//	PolyData polyData;
	//	std::vector<Vertex> vertices;
	//	std::vector<unsigned short> indices;
	//	VertexBufferSet vertSet;
	//	IndexBufferSet indexSet;
	//};
	//
	//struct Object3DHeapData 
	//{
	//	std::vector<ComPtr<ID3D12Resource>> constBuffer;
	//	ComPtr<ID3D12Resource> textureBuffer;
	//	std::vector<DirectX::XMVECTOR>position;
	//	DirectX::XMFLOAT3 angle;
	//	DirectX::XMFLOAT3 scale;
	//};
	//
	//
	//struct Object2DData
	//{
	//	VertexBufferSet vertSet;
	//	ComPtr<ID3D12Resource> constSet;
	//	ComPtr<ID3D12Resource> textureBuffer;
	//	DirectX::XMVECTOR position;
	//	DirectX::XMFLOAT3 angle;
	//	DirectX::XMFLOAT3 scale;
	//
	//};

#pragma region ポストエフェクト
	struct PostEffectConstData
	{
		DirectX::XMMATRIX worldMat;
	};

#pragma endregion


#pragma endregion


#pragma region システム系
	struct CameraData
	{
		float fovY;
		float nearNumber;
		float farNumber;
		//初期位置
		DirectX::XMFLOAT3 eye;
		DirectX::XMFLOAT3 target;
		DirectX::XMFLOAT3 up;
		//現在位置(行列で計算するやつ)
		DirectX::XMFLOAT3 nowEye;
		DirectX::XMFLOAT3 nowTarget;
		DirectX::XMFLOAT3 nowUp;
		DirectX::XMFLOAT3 eyeAngle;
		DirectX::XMFLOAT3 targetAngle;
		DirectX::XMFLOAT3 upAngle;
	};
#pragma endregion



#pragma region パイプラインとルートシグネチャ
	//struct PipelineSet
	//{
	//	ComPtr<ID3D12RootSignature> rootSignature;
	//	ComPtr<ID3D12PipelineState> pipelineState;
	//	ComPtr<ID3DBlob> errorBlob;
	//};
#pragma endregion
}
