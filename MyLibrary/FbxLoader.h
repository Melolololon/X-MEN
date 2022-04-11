#pragma once

#include"ModelData.h"

#include<string>
#include<d3dx12.h>
#include<fbxsdk.h>

#ifdef _DEBUG
#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"libxml2-md.lib")
#pragma comment(lib,"zlib-md.lib")
#else
#pragma comment(lib,"libfbxsdk-mt.lib")
#pragma comment(lib,"libxml2-mt.lib")
#pragma comment(lib,"zlib-mt.lib")
#endif // _DEBUG

namespace MelLib
{
	class FbxLoader final
	{
		// インデックスでマテリアルとか取得するから、ロード数とかカウントするようにしたほうがいいかも


	private:
		FbxLoader();
		~FbxLoader();

		bool initializeFlag = false;

		ID3D12Device* device = nullptr;

		FbxManager* fbxManager = nullptr;

		FbxImporter* fbxImporter = nullptr;

		std::string modelDirectryPath;

		/// <summary>
		/// ノード構造解析用関数
		/// </summary>
		/// <param name="model">モデルのポインタ</param>
		/// <param name="fbxNode">解析するノード</param>
		/// <param name="parentNode">親ノード</param>
		void ParseNodeRecursive(ModelData* fbxModel, FbxNode* fbxNode, Node* parentNode = nullptr);

		/// <summary>
		/// メッシュ読み込み関数
		/// </summary>
		/// <param name="">モデル</param>
		/// <param name="node">ノード</param>
		void ParseMesh(ModelData* fbxModel, FbxNode* node,Node* meshNode);

		/// <summary>
		/// 頂点読み込み
		/// </summary>
		/// <param name="model">モデル</param>
		/// <param name="fbxMesh">メッシュ</param>
		void ParseMeshVertices(ModelData* fbxModel, FbxMesh* fbxMesh, Node* meshNode,const std::string& name);

		/// <summary>
		/// 面情報(インデックス、UV、法線)読み取り
		/// </summary>
		/// <param name="model">モデル</param>
		/// <param name="fbxMesh">メッシュ</param>
		void ParseMeshFaces(ModelData* fbxModel, FbxMesh* fbxMesh, const std::string& name);

		/// <summary>
		/// マテリアル読み取り
		/// </summary>
		/// <param name="model">モデル</param>
		/// <param name="fbxNode">ノード</param>
		void ParseMaterial(ModelData* fbxModel, FbxNode* fbxNode, const std::string& name);

		/// <summary>
		/// スキニング情報読み取り
		/// </summary>
		/// <param name="model"></param>
		/// <param name="fbxMesh"></param>
		void ParseSkin(ModelData* fbxModel, FbxMesh* fbxMesh, FbxNode* node, const std::string& name);

		void SetParentBone(ModelData* fbxModel, FbxNode* node,FbxNode* parentNode);


		std::string ExtractFileName
		(
			const std::string& path,
			std::string* directry,
			std::string* fileName
		);

	public:
		FbxLoader(const FbxLoader& model) = delete;
		FbxLoader& operator =(const FbxLoader& model) = delete;
		static FbxLoader* GetInstance();

		/// <summary>
		/// 初期化します
		/// </summary>
		/// <param name="device">デバイス</param>
		void Initialize(ID3D12Device* device);

		void Finalize();

		/// <summary>
		/// fbxを読み込みます
		/// </summary>
		/// <param name="modelPath">モデルのパス</param>
		/// <param name="fbxModel">モデルのポインタ</param>
		bool LoadFbxModel(const std::string& modelPath, ModelData* fbxModel);

		/// <summary>
		/// Fbxの行列からXMMATRIXに変換するクラス
		/// </summary>
		/// <param name="dst"></param>
		/// <param name="src"></param>
		static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

		bool GetInitializeFlag()const { return initializeFlag; }
	};

}