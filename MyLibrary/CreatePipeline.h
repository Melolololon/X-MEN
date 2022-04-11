//#pragma once
//#include<vector>
//#include<d3d12.h>
//#include<dxgi1_6.h>
//#include<d3dcompiler.h>
//#pragma comment(lib,"d3d12.lib")
//#pragma comment(lib,"dxgi.lib")
//#pragma comment(lib,"d3dcompiler.lib")
//#include"DirectXStruct.h"
//#include"ShaderData.h"
//
//
//
//
//class CreatePipeline
//{
//private:
//	ID3D12Device* dev;
//	
//	D3D12_INPUT_ELEMENT_DESC inputLayout[3];
//	D3D12_INPUT_ELEMENT_DESC spriteInputLayout[2];
//
//	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayoutVector;
//	D3D12_INPUT_ELEMENT_DESC* userInputLayout;
//
//public:
//	CreatePipeline(ID3D12Device* dev);
//	~CreatePipeline();
//
//	void CreateRootSigneture(D3D12_ROOT_SIGNATURE_DESC rootSigDesc, ID3D12RootSignature** rootSigneture);
//	void CreatePipelineState(int dimention, D3D12_GRAPHICS_PIPELINE_STATE_DESC pDesc, ID3D12PipelineState** pipeline);
//	
//	bool CreateUserPipeline
//	(
//		int dimention,
//		ShaderData vShaderData,
//		ShaderData gSyaderData,
//		ShaderData pShaderData,
//		D3D12_GRAPHICS_PIPELINE_STATE_DESC pDesc, 
//		ID3D12PipelineState** pipeline,
//		bool useUserInputLayout);
//
//	//インプットレイアウトのセットと、パイプラインの作成まとめてもいいかも
//	//boolの部分をstd::vector<InputLayoutData>*にする?
//
//	/// <summary>
///// インプットレイアウトの設定をします
///// </summary>
///// <param name="semantics">セマンティクス名</param>
///// <param name="num">データ数(Vector3の場合、三次元なので3を入力)</param>
//	void SetInputLayout(const std::vector<InputLayoutData>& inputLayoutData);
//
//	/// <summary>
///// 現在のインプットレイアウトを削除します
///// </summary>
//	void DeleteInputLayout();
//};
