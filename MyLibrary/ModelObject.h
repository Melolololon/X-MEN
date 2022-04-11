#pragma once
#include"Vector.h"
#include"Color.h"
#include"DirectXStruct.h"

#include<DirectXMath.h>
#include<vector>
#include<memory>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>


#include"ModelData.h"
#include"PipelineState.h"
#include"Texture.h"
#include"Camera.h"
#include"RenderTarget.h"
#include"BufferData.h"
#include"CollisionType.h"
#include"Material.h"
//AddColorとかまとめよう!

namespace MelLib
{
	
	//モデルの座標などをまとめたもの
	class ModelObject
	{
	private:

	private:

		static std::unordered_map<std::string, std::unique_ptr<ModelObject>>pModelObjects;

		static ID3D12Device* device;
		static std::vector<ID3D12GraphicsCommandList*>cmdLists;
		static ComPtr<ID3D12RootSignature>rootSignature;
	
		std::vector<std::string>objectNames;

		//[モデル内のオブジェクトごと]
		std::unordered_map < std::string, Material*>materials;

#pragma region ボーンとアニメーションの情報
		static const UINT BONE_MAX = 128;
		struct SkinConstBufferData
		{
			DirectX::XMMATRIX bones[BONE_MAX];
		};

		struct FbxAnimationTimes
		{
			FbxTime startTime;
			FbxTime endTime;
			//1フレームの時間
			FbxTime frameTime;
		};

		//fbxモデルのアニメーション用の情報をまとめたもの
		struct FbxAnimationData
		{
			FbxAnimationTimes animationTimes;
			FbxTime currentTime;
			int timeMag = 1;
			std::string currentAnimationName;
		};


		//[ボーンごと]
		std::vector<BoneData>boneDatas;
		std::vector<ParentBoneData> parentBoneDatas;//親ボーンと影響度

		FbxAnimationData fbxAnimationData;
		bool isAnimation = false;
		bool animationEndStop = false;
		bool animationReverse = false;
		bool animationEnd = false;
#pragma endregion

		//定数バッファ
		static const int CONST_BUFFER_REGISTER = 0;
		std::vector<ComPtr<ID3D12Resource>> constBuffer;//メイン(基本的な情報)

		static const int MATERIAL_BUFFER_REGISTER = 2;

		static const int COLOR_MATERIAL_BUFFER_REGISTER = 4;

		static const int USER_BUFFER_REGISTER = 1;
		std::vector<ComPtr<ID3D12Resource>> userConstBuffer;//ユーザー
		ConstBufferData userConstBufferData;

		static const int MODEL_BUFFER_REGISTER = 3;
		// モデルのオブジェクトごとに生成するように変更すること 2021 12/8
		std::vector<ComPtr<ID3D12Resource>> modelConstBuffer;//モデル特有(アニメーション関係の情報)
		ConstBufferData modelConstBufferData;

		static const int TEXURE_ROOTPARAM_NUM = 5;

		//定数にセットする座標などの値
		//[モデル内のオブジェクト数]
		std::unordered_map < std::string , ModelConstData > modelConstDatas;

		//モデル(クリエイトしたら割り当てる)
		ModelData* pModelData = nullptr;
		UINT modelFileObjectNum = 0;



		//テクスチャ未セット時にセットする透明のテクスチャバッファ
		static ComPtr<ID3D12Resource>colorZeroTexBuffer;

		//こいつ宣言してModelObjectのvectorとかresizeするとエラー出る。対策して
		//ModelDataを持たせないでモデルデータ渡した時に使うデータだけ持ってくればいい?
		//一時的にModelData作ってModelObjectにセットしてすぐ消しちゃう
		//渡すとなると、生成するごとに頂点のコピーが必要になる(コピーしないとカットしたモデルの頂点参照できない)
		//直接ModelObjectに頂点とか渡して作れるようにしてもいいかも
		//頂点とインデックス関係のものだけをまとめたクラスを作って、それをこれに持たせるのもあり
		
		// //ModelDataないとヒープ用意できないから、
		//マテリアルとかのコピーコンストラクタ作ったほうがいいかも
		std::unique_ptr<ModelData> catFrontModelData;
		std::unique_ptr<ModelData> catBackModelData;
		//ModelData* catFrontModelData;
		//ModelData* catBackModelData;

	private:

		void CreateConstBuffer();

		void DrawCommonProcessing(const std::string& rtName);
		void MapConstData(const Camera* camera);
		void SetCmdList();

		void FbxAnimation();
	public:

		//nullptr渡される可能性を考えると、boolをreturnできるようにしたほうがいい?
		ModelObject() {}
		ModelObject( ModelObject& obj);
		ModelObject& operator= (ModelObject& obj);
		~ModelObject() {}

		static bool Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList);


		static ModelObject* Get(const std::string& name) { return pModelObjects[name].get(); }
		static void Delete(const std::string& name);

		/// <summary>
		/// アニメーションなどの更新処理を行います
		/// </summary>
		void Update();
		void Draw(const std::string& rtName = RenderTarget::GetMainRenderTargetNama());

#pragma region 生成

#pragma region モデルで生成
		ModelObject(ModelData* pModelData, ConstBufferData* userConstBufferData);
		static bool Create(ModelData* pModelData, ConstBufferData* userConstBufferData, const std::string& name);
		bool Create(ModelData* pModelData, ConstBufferData* userConstBufferData = nullptr);
#pragma endregion モデルで生成


#pragma region プリミティブモデル生成
	

#pragma endregion プリミティブモデル生成


#pragma endregion 生成

#pragma region メッシュカット
		/// <summary>
		/// モデルを平面に応じて切断します。
		/// </summary>
		/// <param name="plane">平面</param>
		/// <param name="pFront">平面の表側にあるモデル情報を格納するModelDataのポインタ</param>
		/// <param name="pBack">平面の裏側にあるモデル情報を格納するModelDataのポインタ<</param>
		/// <param name="createCrossSection">断面を形成するかどうか</param>
		/// <returns>切断できたかどうか</returns>
		bool MeshCat(const PlaneData& plane,ModelData*& pFront, ModelData*& pBack,const bool createCrossSection);
#pragma endregion


#pragma region セット

#pragma region 操作見た目変更

#pragma region 操作


		void SetPosition(const Vector3& position,const std::string& name = "");
		void SetScale(const Vector3& scale, const std::string& name = "");
		void SetAngle(const Vector3& angle, const std::string& name = "");



#pragma endregion

#pragma region 色
		void SetAddColor(const Color& color, const std::string& name = "");
		void SetSubColor(const Color& color, const std::string& name = "");
		void SetMulColor(const Color& color, const std::string& name = "");
#pragma endregion


#pragma region アニメーション

		void SetAnimationPlayFlag(const bool flag) { isAnimation = flag; }

		/// <summary>
		/// アニメーションをリセットします。
		/// </summary>
		void SetAnimationFrameStart() {fbxAnimationData.currentTime = 0;}

		/// <summary>
		/// アニメーションの現在のフレームをアニメーション終了時のフレームにします。
		/// </summary>
		void SetAnimationFrameEnd() { fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime; }

		void SetCurrentFream(const UINT fream);

		void SetAnimationSpeedMagnification(const unsigned int magnification) { fbxAnimationData.timeMag = magnification; }

		/// <summary>
		/// アニメーションを逆再生するかどうかを設定します。
		/// </summary>
		/// <param name="flag"></param>
		void SetAnimationReversePlayBack(const bool flag);

		void SetAnimation(const std::string& name);

		void SetAnimationEndStopFlag(const bool flag) { animationEndStop = flag; }

		/// <summary>
		/// アニメーションのフレームをセットします。
		/// </summary>
		/// <param name="frame"></param>
		void SetAnimationFrame(const unsigned int frame) { fbxAnimationData.currentTime.SetFrame(frame, FbxTime::eFrames60); }

#pragma endregion


#pragma endregion

		void SetMaterial(Material* mtl,const std::string& name = "");




#pragma endregion

#pragma region ゲット

		//この辺constにする

		ModelData* GetPModelData() { return pModelData; }
		Material* GetPMaterial(const std::string& name = "");

#pragma region 操作見た目変更

#pragma region 操作
		
		//今はとりあえず全部値が一緒なので、0のやつを返してる
		Vector3 GetPosition(const std::string& name = "")const;

		Vector3 GetAngle(const std::string& name = "")const;
		Vector3 GetScale(const std::string& name = "")const;
#pragma endregion
#pragma endregion


#pragma region アニメーション


		std::string GetCurrentAnimationName()const { return fbxAnimationData.currentAnimationName; }

		/// <summary>
		/// アニメーションが逆生成中かどうかを取得します。
		/// </summary>
		/// <returns></returns>
		bool GetAnimationReversePlayBack()const { return animationReverse; }

		/// <summary>
		/// アニメーションが終了しているかどうかを取得します。
		/// </summary>
		/// <returns></returns>
		bool GetAnimationEndFlag()const { return animationEnd; }
		//{ return fbxAnimationData.currentTime == fbxAnimationData.animationTimes.endTime; }


		/// <summary>
		///	アニメーションの現在のフレームを取得します。
		/// </summary>
		/// <returns></returns>
		unsigned int GetAnimationFrame()const { return fbxAnimationData.currentTime.GetFrameCount(FbxTime::eFrames60); }
		
		unsigned int GetAnimationFrameMax()const { return fbxAnimationData.animationTimes.endTime.GetFrameCount(FbxTime::eFrames60); }
#pragma endregion

		//コンピュートシェーダーで計算したほうがいい。
		//できそうなら描画時に頂点シェーダーで計算した結果を持ってきたほうがいい?
		// 戻り値がumap版のやつ作ってもいいかも
		/// <summary>
		/// 頂点座標を取得します。
		/// </summary>
		/// <param name="scaleImpact"></param>
		/// <param name="angleImpact"></param>
		/// <param name="transformImpact"></param>
		/// <param name=""></param>
		/// <returns></returns>
		std::vector<std::vector<Vector3>>GetVerticesData
		(
			const bool scaleImpact,
			const bool angleImpact,
			const bool transformImpact
		);

		/// <summary>
		/// モデルのポリゴンを当たり判定に使えるデータに変換して返します。
		/// </summary>
		/// <returns>モデルを元にセットした三角形の情報の配列</returns>
		std::vector<std::vector<TriangleData>>GetModelTriangleData()const;
#pragma endregion

		/// <summary>
		/// 指定したボーンとメッシュの行列を渡した座標に乗算して返します。
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="boneName">ボーン名</param>
		/// <param name="meshName">メッシュ名</param>
		/// <returns>行列乗算後の座標</returns>
		Vector3 CalcAnimationPosition
		(
			const Vector3& pos,float weigth, const std::string& boneName,const std::string& meshName
		, const MelLib::Vector3& startPos, const MelLib::Vector3& startAngle, const MelLib::Vector3& startScale)const;

	};
}
