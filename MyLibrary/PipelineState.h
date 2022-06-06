#pragma once
#include"DirectXStruct.h"
#include"ShaderData.h"
#include<string>
#include<unordered_map>
#include<memory>

//パイプラインは別にモデルみたいにstaticのアクセス関数とかいらない?
//それとも、シーンをまたいで使う可能性あるからいる?

namespace MelLib
{
	enum class PipelineStateType
	{
		MODEL,
		SPRITE,
		RENDER_TARGET,
	};
	class PipelineState
	{

	private:
		static std::unordered_map<std::string, std::unique_ptr<PipelineState>>pPipelineState;
		static ID3D12Device* device;


		static void SetPipelineDesc
		(
			const DrawOption& drawData,
			D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc,
			const PipelineStateType type,
			const int renderTargetNum
		);

		static ID3D12RootSignature* modelRootSignature;
		static ID3D12RootSignature* spriteRootSignature;
		static ID3D12RootSignature* renderTargetRootSignature;



		ComPtr<ID3D12PipelineState>pipelineState;
		DrawOption drawData;
		ShaderDataSet shaderSet;
		PipelineStateType pipelineType = PipelineStateType::MODEL;
		std::unique_ptr<std::vector<InputLayoutData>> inputLayoutData = nullptr;
		int renderTargetNum = 1;
	public:
		PipelineState() {}
		PipelineState(PipelineState& pso);
		PipelineState& operator= (PipelineState& pso);

		~PipelineState() {}

		static bool Initialize
		(
			ID3D12Device* dev
		);

		/// <summary>
		/// パイプラインを生成します
		/// </summary>
		/// <param name="drawData">パイプライン情報</param>
		/// <param name="shaderSet">シェーダーのデータをまとめたもの</param>
		/// <param name="pipelineType">何のパイプラインを生成するか</param>
		/// <param name="inputLayoutData">インプットレイアウト情報(指定しない場合は、nullptrを渡す)</param>
		/// <param name="renderTargetNum">同時にどのくらい出力するか</param>
		/// <param name="name">登録名</param>
		/// <returns></returns>
		static void Create
		(
			const DrawOption& drawData,
			const ShaderDataSet& shaderSet,
			const PipelineStateType pipelineType,
			const std::vector<InputLayoutData>* inputLayoutData,
			const int renderTargetNum,
			const std::string& name
		);

		static PipelineState* Get(const std::string& name) { return pPipelineState[name].get(); }
		static void Delete(const std::string& name);

		/// <summary>
		/// パイプラインを生成します
		/// </summary>
		/// <param name="drawData">パイプライン情報</param>
		/// <param name="shaderSet">シェーダーのデータをまとめたもの</param>
		/// <param name="pipelineType">何のパイプラインを生成するか</param>
		/// <param name="inputLayoutData">インプットレイアウト情報(指定しない場合は、nullptrを渡す)</param>
		/// <param name="renderTargetNum">同時にどのくらい出力するか</param>
		/// <returns></returns>
		bool CreatePipeline
		(
			const DrawOption& drawData,
			const ShaderDataSet& shaderSet,
			const PipelineStateType pipelineType,
			const std::vector<InputLayoutData>* inputLayoutData,
			const int renderTargetNum
		);





		ComPtr<ID3D12PipelineState> GetPipelineState()
		{
			return pipelineState;
		}

		/// <summary>
		/// ライブラリで使用しているパイプラインの情報を引数dataにセットします。
		/// </summary>
		/// <param name="data"></param>
		/// <param name="type"></param>
		static DrawOption GetDefaultDrawData( const PipelineStateType type);

		static void SetModelRootSignature(ID3D12RootSignature* sig) { modelRootSignature = sig; }
		static void SetSpriteRootSignature(ID3D12RootSignature* sig) { spriteRootSignature = sig; }
		static void SetRenderTargetRootSignature(ID3D12RootSignature* sig) { renderTargetRootSignature = sig; }

	};
}
