#pragma once

#include<memory>
#include<vector>
#include<array>
#include<map>

#include<DirectXMath.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>

#include"DirectXStruct.h"
#include"Texture.h"
#include"Color.h"
#include"BufferData.h"
#include"CollisionDetectionData.h"

#include"Material.h"

// これ読み込めないエラーが新規プロジェクト開発時に出たらx64になってるか確認しよう!
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
	struct Node
	{
		std::string nodeName;

		DirectX::XMVECTOR scaling = { 1,1,1,0 };
		DirectX::XMVECTOR rotation = { 1,1,1,0 };
		DirectX::XMVECTOR translation = { 1,1,1,0 };

		// 上3つを組み安房得たやつ
		DirectX::XMMATRIX transform;

		// 親ノードの行列も乗算したやつ
		DirectX::XMMATRIX globalTransform;

		Node* parentNode = nullptr;
	};


	//objとかがfbxのデータ持ってるのメモリもったいないから、
	//構造体でまとめて、fbxの時だけnewでメモリ確保するようにすればもったいなくない

	//モデルの頂点、インデックス、テクスチャをまとめたクラス
	class ModelData
	{
	public:
	
		enum class ModelFormat
		{
			MODEL_FORMAT_NONE,
			MODEL_FORMAT_PRIMITIVE,
			MODEL_FORMAT_OBJ,
			MODEL_FORMAT_FBX,
		};


#pragma region fbx構造体
		//fbxのボーン情報をまとめたもの
		struct FbxBone
		{
			//ボーン名
			std::string boneName;
			//初期姿勢の逆行列
			DirectX::XMMATRIX invInitialPose;

			FbxBone* parentBone = nullptr;

			//ボーン情報
			FbxCluster* fbxCluster;

			FbxBone(const std::string& name)
			{
				this->boneName = name;
			}
		};

		//fbxのアニメーションに必要なFbxTimeをまとめた構造体
		FbxTime freamTime;
	

#pragma endregion


	private:

#pragma region obj構造体

		//objのボーン情報をまとめたもの
		struct ObjBone
		{
			std::vector<Vector3>bonePosition;
		};

#pragma endregion

#pragma region fbx構造体


		//fbxのデータをまとめたもの
		struct FbxData
		{
			FbxScene* fbxScene = nullptr;

			//モデルのノード
			std::unordered_map<std::string,Node>nodes;

			//メッシュを持つノード
			Node* meshNode = nullptr;

			// mapなら順番保持されるからそっちにしてもいいかも
			//std::unordered_map<std::string, FbxBone>bones_u;
			std::vector<FbxBone>bones;

			//FbxAnimationTimes animationTimes;

			// FbxAnimStackを取得するための番号
			std::unordered_map<std::string, int>animStackNum;
			// アニメーション情報を取得するための名前
			std::unordered_map<std::string, std::string>animationDataGetName;
		};


#pragma endregion

		//モデルを格納する配列
		static std::unordered_map<std::string, std::unique_ptr<ModelData>>pModelDatas;
		//プリミティブモデルを格納する配列
		static std::unordered_map<ShapeType3D, std::unique_ptr<ModelData>>pPrimitiveModelDatas;

		//プリミティブモデル、マテリアルがnullptrのモデルデータに割り当てられるマテリアル
		static std::unique_ptr<ADSAMaterial>defaultMaterial;


		static ID3D12Device* device;

#pragma region バッファ_ヒープ

		//頂点
		std::unordered_map<std::string, VertexBufferSet> vertexBufferSet;
		//インデックス
		std::unordered_map <std::string, IndexBufferSet> indexBufferSet;
		ComPtr<ID3D12DescriptorHeap>textureDescHeap;
		std::unordered_map<std::string, ComPtr<ID3D12Resource>> textureBuffers;

#pragma endregion

		//一斉削除対象フラグ
		bool batchDeletionFlag = false;

#pragma region モデル情報
		ModelFormat modelFormat = ModelFormat::MODEL_FORMAT_NONE;

		// モデル名
		std::string modelName;

		std::string modelPath;

		// モデルファイルのオブジェクト名一覧
		std::vector<std::string>objectNames;

		//マテリアルにセットするテクスチャ
		std::unordered_map<std::string, std::unique_ptr<Texture>>pTexture;
		//マテリアル
		std::unordered_map<std::string, std::unique_ptr<ADSAMaterial>> material;

		//テクスチャを使わない場合の色
		Color color;

		//モデルファイルに何個モデルがあるか
		UINT modelFileObjectNum = 1;
		std::unordered_map<std::string, std::vector<FbxVertex>>vertices;
		//スムーズシェーディング用法線
		std::unordered_map<std::string, std::vector<DirectX::XMFLOAT3>> smoothNormal;
		//上下左右前後の最高(最低)座標の値(上下左右前後の順に格納)
		std::unordered_map < std::string, std::array<float, 6>>directionMaxPos;

		std::unordered_map<std::string, std::vector<USHORT>> indices;

		// 複数メッシュに対応させる場合、これ順序保持できるmapにして、名前でこの行列とれるようにする(座標回転関数で名前指定して使用するため)
		// メッシュのグローバルトランスフォーム行列の配列)
		std::map<std::string, DirectX::XMMATRIX>meshGlobalTransform;

		UINT boneNum = 0;

		ObjBone objData;
		FbxData fbxData;


		// モデリングソフトから読み取った判定を格納
#pragma region 読み取った判定情報
		// オブジェクト名「Collision_形状名_判定名」だった場合、モデルを判定情報として読み取る
		// 複数の座標を持つ判定(カプセルなど)は、上記の「」内の名前に加え、「_番号」を付ける
		// 例 球「Collision_Sphere_Main」
		// 例 カプセル「Collision_Capsule_Sword_01」「Collision_Capsule_Sword_02」これでカプセル1個分
		
		// ModelObjectで移動とかしたらこいつらも動かす
		// 描画もできるようにする
		
		std::unordered_map<std::string, SphereData>sphereDatas;
		std::unordered_map<std::string, BoxData>boxDatas;
		std::unordered_map<std::string, Segment3DData>segmentDatas;
		std::unordered_map<std::string, CapsuleData>capsuleDatas;
		std::unordered_map<std::string, PlaneData>planeDatas;
		std::unordered_map<std::string, BoardData>boardDatas;
#pragma endregion


#pragma endregion

#pragma region 頂点

		/// <summary>
		/// 頂点バッファ、ビューの生成を行います。
		/// </summary>
		/// <param name="vertexSize"></param>
		/// <param name="vertices"></param>
		void CreateVertexBufferSet
		(
			const size_t vertexSize,
			const std::unordered_map < std::string, size_t>& vertexNum
		);

		void MapVertices(void** data, const std::string& name);
		void UnmapVertices(const std::string& name);
#pragma endregion

#pragma region インデックス


		/// <summary>
		/// インデックスバッファ、ビューの生成を行います。
		/// </summary>
		/// <param name="indices"></param>
		void CreateIndexBufferSet
		(
			const std::unordered_map<std::string, std::vector<USHORT>>& indices
		);

		void MapIndices(const std::unordered_map<std::string, std::vector<USHORT>>& indices);
#pragma endregion




		private:

		static void CreatePrimitiveModel();

		// ライブラリ実装のプリミティブモデルの法線計算関数
		static void CalcPrimitiveModelNormal(std::vector<FbxVertex>& vertices,const std::vector<USHORT>& indices);
		
		void CreateModel();

		/// <summary>
		/// モデルの頂点データ、マテリアルを読み込みます。
		/// </summary>
		/// <param name="path"></param>
		/// <param name="name"></param>
		bool LoadModel(const std::string& path, const std::string& name);


		/// <summary>
		/// 頂点インデックスバッファテクスチャバッファの生成とインデックスとテクスチャのMapを行います。
		/// </summary>
		/// <param name="vertexSize"></param>
		/// <param name="vertexNum"></param>
		/// <param name="vertices"></param>
		/// <param name="indices"></param>
		void BufferPreparationSetTexture
		(
			const size_t vertexSize,
			const std::unordered_map < std::string, size_t>& vertexNum,
			const std::unordered_map < std::string, std::vector<USHORT>>& indices
		);

		void BufferPreparationSetColor
		(
			const size_t vertexSize,
			const std::unordered_map < std::string, size_t>& vertexNum,
			const std::unordered_map < std::string, std::vector<USHORT>>& indices
		);


		static std::unordered_map < std::string, std::array<float, 6>>CalcDirectionMaxPosition
		(
			const std::unordered_map < std::string, std::vector<FbxVertex>>& vertices,
			const std::vector<std::string>& objectNames
		);

		/// <summary>
		/// モデルファイルから読み取った判定情報を追加する関数
		/// </summary>
		void AddLoadCollisionData(const std::string& objectName);

	public:

		ModelData() {}
		ModelData(ModelData& data);
		ModelData& operator=(ModelData& data);

		~ModelData();

		/// <summary>
		/// 頂点情報を元にモデルデータを生成します。
		/// </summary>
		/// <param name="vertices"></param>
		/// <param name="indices"></param>
		/// <param name=""></param>
		/// <param name="batchDeletionFlag"></param>
		/// <param name="name"></param>
		/// <returns></returns>
		static bool Create
		(
			const std::unordered_map < std::string, std::vector<FbxVertex>>&vertices, 
			const std::unordered_map < std::string, std::vector<USHORT>>& indices, 
			const bool batchDeletionFlag, 
			const std::string& name
		);
		
		static bool Create
		(
			const std::vector< std::vector<FbxVertex>>& vertices, 
			const std::vector< std::vector<USHORT>>&indices, 
			const bool batchDeletionFlag, 
			const std::string& name
		);

		void Create
		(
			const std::unordered_map < std::string, std::vector<FbxVertex>>& vertices,
			const std::unordered_map < std::string, std::vector<USHORT>>& indices
		);

		void Create
		(
			const std::vector<std::vector<FbxVertex>>& vertices,
			const std::vector<std::vector<USHORT>>& indices
		);

		/// <summary>
		/// モデルを読み込みます。
		/// </summary>
		/// <param name="path">パス</param>
		/// <param name="batchDeletionFlag">一括削除対象かどうか</param>
		/// <param name="name">登録名</param>
		/// <returns></returns>
		static bool Load(const std::string& path,const bool batchDeletionFlag, const std::string& name);

		/// <summary>
		///	モデルデータを取得します。
		/// </summary>
		/// <param name="name">モデルの登録名</param>
		/// <returns></returns>
		static ModelData* Get(const std::string& name) { return pModelDatas[name].get(); }

		/// <summary>
		/// モデルデータを取得します。
		/// </summary>
		/// <param name="type">取得したいモデルの形状</param>
		/// <returns></returns>
		static ModelData* Get(const ShapeType3D type) { return pPrimitiveModelDatas[type].get(); }

		/// <summary>
		/// モデルデータを削除します。
		/// </summary>
		/// <param name="name"></param>
		static void Delete(const std::string& name);
		
		/// <summary>
		/// 一括削除の対象になっているモデルデータを削除します。
		/// </summary>
		static void BatchDeletion();



#pragma region 開発者用関数
		static void Initialize(ID3D12Device* pDevice);

		/// <summary>
		/// インデックスを取得。
		/// </summary>
		/// <returns></returns>
		std::vector<std::vector<USHORT>> GetIndices()const;

		/// <summary>
		/// 頂点バッファを取得。
		/// </summary>
		/// <returns></returns>
		std::vector<VertexBufferSet> GetVertexBufferSet()const;

		/// <summary>
		/// インデックスバッファの取得。
		/// </summary>
		/// <returns></returns>
		std::vector<IndexBufferSet> GetIndexBufferSet()const;

		/// <summary>
		/// ディスクリプタヒープの取得。
		/// </summary>
		/// <returns></returns>
		ID3D12DescriptorHeap* GetTextureDesctiptorHeap()const { return textureDescHeap.Get(); }

		void GetAnimationTimeData(int index, FbxTime& start, FbxTime& end);
		void GetAnimationTimeData(const std::string& name, FbxTime& start, FbxTime& end);

		DirectX::XMMATRIX GetMeshGlobalTransform(const std::string& name) { return meshGlobalTransform[name]; }
		std::vector<DirectX::XMMATRIX>GetMeshGlobalTransforms();

#pragma region fbx関係


		const std::vector<FbxBone>& GetFbxBone() const { return fbxData.bones; }

		/// <summary>
		/// ボーンを取得します。
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		const FbxBone& GetFbxBone(const std::string& name)const;

		Node GetNode(const std::string& name) {return fbxData.nodes[name]; }

		/// <summary>
		/// モデルのFbxAnimationTimesを返します。
		/// </summary>
		/// <returns></returns>
		//const FbxAnimationTimes& GetFbxAnimationTimes()const { return fbxData.animationTimes; }


		void SetFbxAnimStack(const std::string& name);

#pragma endregion

#pragma region obj関係

		const std::vector<Vector3>& GetObjBonePosition()const { return objData.bonePosition; }
#pragma endregion


#pragma endregion

#pragma region セット

#pragma endregion

#pragma region ゲット

		std::string GetModelPath()const { return modelPath; }
		/// <summary>
		/// モデルに含まれているオブジェクト名を取得します。
		/// </summary>
		/// <returns></returns>
		std::vector<std::string>GetObjectNames()const { return objectNames; }

		/// <summary>
		/// モデルのフォーマットを取得します。
		/// </summary>
		/// <returns></returns>
		ModelFormat GetModelFormat() const { return modelFormat; }

		/// <summary>
		/// ボーン数を取得します。
		/// </summary>
		/// <returns></returns>
		UINT GetBoneNumber() const { return boneNum; }

		/// <summary>
		/// モデルファイルに含まれているオブジェクト(モデル)の数を取得します。
		/// </summary>
		/// <returns></returns>
		UINT GetModelFileObjectNumber()const { return modelFileObjectNum; }

		//ポインタ返さないと、2回目呼び出した時にmetadataがちゃんと代入できてないのか初期化してないときと同じになるからポインタ返してる
		/// <summary>
		/// 読み込んだモデルのマテリアルのポインタを取得します。
		/// </summary> 
		/// <returns></returns>
		std::vector<ADSAMaterial*> GetPMaterial();

		static ADSAMaterial* GetDefaultMaterial() { return defaultMaterial.get(); }

		/// <summary>
		/// 上下左右前後の最高値を取得します。
		/// </summary>
		/// <returns></returns>
		std::vector<std::array<float, 6>>GetDirectionMaxPosition() const;

		/// <summary>
		/// 頂点を取得します。
		/// </summary>
		/// <returns></returns>
		std::vector<std::vector<FbxVertex>>GetVertices()const;

		/// <summary>
		/// 頂点座標を取得します。
		/// </summary>
		/// <returns>頂点座標</returns>
		std::vector<std::vector<Vector3>>GetVerticesPosition()const;

#pragma endregion

		//フレンドクラスは、privateの変数、関数にアクセスできる
		friend class FbxLoader;
	};

}