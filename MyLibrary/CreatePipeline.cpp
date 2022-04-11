//#include "CreatePipeline.h"
//
//
//using namespace melLib;
//
//CreatePipeline::CreatePipeline(ID3D12Device* dev)
//{
//	this->dev = dev;
//}
//
//
//CreatePipeline::~CreatePipeline()
//{
//}
//
//
//
//void CreatePipeline::CreateRootSigneture(D3D12_ROOT_SIGNATURE_DESC rootSigDesc,ID3D12RootSignature** rootSigneture)
//{
//	ComPtr<ID3DBlob>rootSigBlob;
//	ComPtr<ID3DBlob>errorBlob;
//
//	auto result = D3D12SerializeRootSignature(
//		&rootSigDesc,
//		D3D_ROOT_SIGNATURE_VERSION_1_0,
//		&rootSigBlob, &errorBlob);
//	
//	result = dev->CreateRootSignature(
//		0,
//		rootSigBlob->GetBufferPointer(),
//		rootSigBlob->GetBufferSize(),
//		IID_PPV_ARGS(rootSigneture));
//
//}
//
//void CreatePipeline::CreatePipelineState(int dimention, D3D12_GRAPHICS_PIPELINE_STATE_DESC pDesc,ID3D12PipelineState** pipeline)
//{
//	HRESULT result;
//	ComPtr<ID3DBlob> vsBlob;
//	ComPtr<ID3DBlob> psBlob;
//	ComPtr<ID3DBlob> gsBlob;
//	ComPtr<ID3DBlob>errorBlob;
//	
//	if (dimention == 0)
//	{
//		spriteInputLayout[0] =
//	{
//		"POSITION",
//		0,
//		DXGI_FORMAT_R32G32B32_FLOAT,
//		0,
//		D3D12_APPEND_ALIGNED_ELEMENT,
//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//		0
//	};
//	spriteInputLayout[1] =
//	{
//		"TEXCOORD",
//		0,
//		DXGI_FORMAT_R32G32_FLOAT,
//		0,
//		D3D12_APPEND_ALIGNED_ELEMENT,
//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//		0
//	};
//	pDesc.InputLayout.pInputElementDescs = spriteInputLayout;
//	pDesc.InputLayout.NumElements = _countof(spriteInputLayout);
//
//	}
//	if (dimention == 1)
//	{
//		inputLayout[0] =
//		{
//				"POSITION",
//				0,
//				DXGI_FORMAT_R32G32B32_FLOAT,
//				0,
//				D3D12_APPEND_ALIGNED_ELEMENT,
//				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//				0
//		};
//		inputLayout[1] =
//		{
//				"TEXCOORD",
//				0,
//				DXGI_FORMAT_R32G32_FLOAT,
//				0,
//				D3D12_APPEND_ALIGNED_ELEMENT,
//				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//				0
//		};
//		inputLayout[2] =
//		{
//				"NORMAL",
//				0,
//				DXGI_FORMAT_R32G32B32_FLOAT,
//				0,
//				D3D12_APPEND_ALIGNED_ELEMENT,
//				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//				0
//		};
//		pDesc.InputLayout.pInputElementDescs = inputLayout;
//		pDesc.InputLayout.NumElements = _countof(inputLayout);
//	}
//
//	if (dimention == 0 )
//	{
//		result = D3DCompileFromFile(
//			L"../MyLibrary/SpriteVertexShader.hlsl",
//			nullptr,
//			D3D_COMPILE_STANDARD_FILE_INCLUDE,
//			"VSmain", "vs_5_0",
//			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//			0,
//			&vsBlob, &errorBlob);
//
//		result = D3DCompileFromFile(
//			L"../MyLibrary/SpritePixelShader.hlsl",
//			nullptr,
//			D3D_COMPILE_STANDARD_FILE_INCLUDE,
//			"PSmain", "ps_5_0",
//			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//			0,
//			&psBlob, &errorBlob);
//
//		pDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
//		pDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
//		pDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
//		pDesc.PS.BytecodeLength = psBlob->GetBufferSize();
//	}
//	if (dimention == 1)
//	{
//		result = D3DCompileFromFile(
//			L"../MyLibrary/BasicVertexShader.hlsl",
//			nullptr,
//			D3D_COMPILE_STANDARD_FILE_INCLUDE,
//			"VSmain", "vs_5_0",
//			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//			0,
//			&vsBlob, &errorBlob);
//
//		result = D3DCompileFromFile(
//			L"../MyLibrary/BasicPixelShader.hlsl",
//			nullptr,
//			D3D_COMPILE_STANDARD_FILE_INCLUDE,
//			"PSmain", "ps_5_0",
//			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//			0,
//			&psBlob, &errorBlob);
//
//		result = D3DCompileFromFile(
//			L"../MyLibrary/BasicGeometryShader.hlsl",
//			nullptr,
//			D3D_COMPILE_STANDARD_FILE_INCLUDE,
//			"GSmain", "gs_5_0",
//			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//			0,
//			&gsBlob, &errorBlob);
//
//		pDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
//		pDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
//		pDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
//		pDesc.PS.BytecodeLength = psBlob->GetBufferSize();
//		pDesc.GS.pShaderBytecode = gsBlob->GetBufferPointer();
//		pDesc.GS.BytecodeLength = gsBlob->GetBufferSize();
//	}
//
//	if (FAILED(result))
//	{
//		std::string errstr;
//		errstr.resize(errorBlob->GetBufferSize());
//
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			errstr.begin());
//		errstr += "\n";
//
//		OutputDebugStringA(errstr.c_str());
//		exit(1);
//	}
//
//	result = dev->CreateGraphicsPipelineState(&pDesc, IID_PPV_ARGS(pipeline));
//}
//
//
//bool CreatePipeline::CreateUserPipeline(
//	int dimention,
//	ShaderData vShaderData,
//	ShaderData gSyaderData,
//	ShaderData pShaderData,
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC pDesc,
//	ID3D12PipelineState** pipeline,
//	bool useUserInputLayout)
//{
//	if(vShaderData.shaderPath == L"NULL" ||
//		pShaderData.shaderPath == L"NULL")OutputDebugString(L"頂点シェーダーとピクセルシェーダーは必ず読み込んでください");
//
//	HRESULT result;
//	ComPtr<ID3DBlob> vsBlob;
//	ComPtr<ID3DBlob> psBlob;
//	ComPtr<ID3DBlob> gsBlob;
//	ComPtr<ID3DBlob> hsBlob;
//	ComPtr<ID3DBlob> dsBlob;
//	ComPtr<ID3DBlob>errorBlob;
//
//#pragma region インプットレイアウト
//
//	if (dimention == 0)
//	{
//		if (!useUserInputLayout)
//		{
//			spriteInputLayout[0] =
//			{
//				"POSITION",
//				0,
//				DXGI_FORMAT_R32G32B32_FLOAT,
//				0,
//				D3D12_APPEND_ALIGNED_ELEMENT,
//				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//				0
//			};
//			spriteInputLayout[1] =
//			{
//				"TEXCOORD",
//				0,
//				DXGI_FORMAT_R32G32_FLOAT,
//				0,
//				D3D12_APPEND_ALIGNED_ELEMENT,
//				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//				0
//			};
//			pDesc.InputLayout.pInputElementDescs = spriteInputLayout;
//			pDesc.InputLayout.NumElements = _countof(spriteInputLayout);
//		}
//		else
//		{
//			userInputLayout = new D3D12_INPUT_ELEMENT_DESC[inputLayoutVector.size()];
//
//			for (UINT i = 0; i < inputLayoutVector.size() - 1; i++)
//			{
//				userInputLayout[i] = inputLayoutVector[i];
//			}
//
//			pDesc.InputLayout.pInputElementDescs = userInputLayout;
//			pDesc.InputLayout.NumElements = inputLayoutVector.size();
//
//			delete userInputLayout;
//		}
//
//	}
//	if (dimention == 1)
//	{
//		if (!useUserInputLayout)
//		{
//			inputLayout[0] =
//			{
//					"POSITION",
//					0,
//					DXGI_FORMAT_R32G32B32_FLOAT,
//					0,
//					D3D12_APPEND_ALIGNED_ELEMENT,
//					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//					0
//			};
//			inputLayout[1] =
//			{
//					"TEXCOORD",
//					0,
//					DXGI_FORMAT_R32G32_FLOAT,
//					0,
//					D3D12_APPEND_ALIGNED_ELEMENT,
//					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//					0
//			};
//			inputLayout[2] =
//			{
//					"NORMAL",
//					0,
//					DXGI_FORMAT_R32G32B32_FLOAT,
//					0,
//					D3D12_APPEND_ALIGNED_ELEMENT,
//					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//					0
//			};
//
//			pDesc.InputLayout.pInputElementDescs = inputLayout;
//			pDesc.InputLayout.NumElements = _countof(inputLayout);
//		}
//		else 
//		{
//			userInputLayout = new D3D12_INPUT_ELEMENT_DESC[inputLayoutVector.size()];
//
//			int count = 0;
//			for (auto& il: inputLayoutVector)
//			{
//				userInputLayout[count] = il;
//				count++;
//			}
//
//			pDesc.InputLayout.pInputElementDescs = userInputLayout;
//			pDesc.InputLayout.NumElements = inputLayoutVector.size();
//
//		}
//	
//	}
//#pragma endregion
//
//#pragma region シェーダー
//
//	if (dimention == 0)
//	{
//		if (vShaderData.shaderPath == L"LIB") 
//		{
//			result = D3DCompileFromFile(
//				L"../MyLibrary/SpriteVertexShader.hlsl",
//				nullptr,
//				D3D_COMPILE_STANDARD_FILE_INCLUDE,
//				"VSmain", "vs_5_0",
//				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//				0,
//				&vsBlob, &errorBlob);
//		}
//		else 
//		{
//			result = D3DCompileFromFile(
//				vShaderData.shaderPath,
//				nullptr,
//				D3D_COMPILE_STANDARD_FILE_INCLUDE,
//				vShaderData.entryPoint, vShaderData.ver,
//				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//				0,
//				&vsBlob, &errorBlob);
//		}
//
//		if (pShaderData.shaderPath != L"LIB")
//		{
//			result = D3DCompileFromFile(
//				L"../MyLibrary/SpritePixelShader.hlsl",
//				nullptr,
//				D3D_COMPILE_STANDARD_FILE_INCLUDE,
//				"PSmain", "ps_5_0",
//				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//				0,
//				&psBlob, &errorBlob);
//		}
//		else 
//		{
//			result = D3DCompileFromFile(
//				pShaderData.shaderPath,
//				nullptr,
//				D3D_COMPILE_STANDARD_FILE_INCLUDE,
//				pShaderData.entryPoint, pShaderData.ver,
//				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//				0,
//				&psBlob, &errorBlob);
//		}
//	
//
//		pDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
//		pDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
//		pDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
//		pDesc.PS.BytecodeLength = psBlob->GetBufferSize();
//	}
//	if (dimention == 1)
//	{
//#pragma region 頂点シェーダー
//
//		if (vShaderData.shaderPath == L"LIB")
//		{
//		
//
//			result = D3DCompileFromFile(
//				L"../MyLibrary/BasicVertexShader.hlsl",
//				nullptr,
//				D3D_COMPILE_STANDARD_FILE_INCLUDE,
//				"VSmain", "vs_5_0",
//				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//				0,
//				&vsBlob, &errorBlob);
//		}
//		else
//		{
//			result = D3DCompileFromFile(
//				vShaderData.shaderPath,
//				nullptr,
//				D3D_COMPILE_STANDARD_FILE_INCLUDE,
//				vShaderData.entryPoint, vShaderData.ver,
//				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//				0,
//				&vsBlob, &errorBlob);
//
//		}
//
//		pDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
//		pDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
//#pragma endregion
//
//#pragma region ピクセルシェーダー
//
//		if (pShaderData.shaderPath == L"LIB")
//		{
//			result = D3DCompileFromFile(
//				L"../MyLibrary/BasicPixelShader.hlsl",
//				nullptr,
//				D3D_COMPILE_STANDARD_FILE_INCLUDE,
//				"PSmain", "ps_5_0",
//				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//				0,
//				&psBlob, &errorBlob);
//		}
//		else
//		{
//			result = D3DCompileFromFile(
//				pShaderData.shaderPath,
//				nullptr,
//				D3D_COMPILE_STANDARD_FILE_INCLUDE,
//				pShaderData.entryPoint, pShaderData.ver,
//				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//				0,
//				&psBlob, &errorBlob);
//		}
//
//		pDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
//		pDesc.PS.BytecodeLength = psBlob->GetBufferSize();
//#pragma endregion
//
//#pragma region ジオメトリシェーダー
//
//		if (gSyaderData.shaderPath == L"LIB") 
//		{
//			result = D3DCompileFromFile(
//				L"../MyLibrary/BasicGeometryShader.hlsl",
//				nullptr,
//				D3D_COMPILE_STANDARD_FILE_INCLUDE,
//				"GSmain", "gs_5_0",
//				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//				0,
//				&gsBlob, &errorBlob);
//
//			pDesc.GS.pShaderBytecode = gsBlob->GetBufferPointer();
//			pDesc.GS.BytecodeLength = gsBlob->GetBufferSize();
//		}
//		else 
//		if (gSyaderData.shaderPath == L"NULL") {}
//		else
//		{
//			result = D3DCompileFromFile(
//				gSyaderData.shaderPath,
//				nullptr,
//				D3D_COMPILE_STANDARD_FILE_INCLUDE,
//				gSyaderData.entryPoint, gSyaderData.ver,
//				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//				0,
//				&gsBlob, &errorBlob);
//
//			pDesc.GS.pShaderBytecode = gsBlob->GetBufferPointer();
//			pDesc.GS.BytecodeLength = gsBlob->GetBufferSize();
//		}
//#pragma endregion
//
//#pragma region ハルシェーダー
//		/*result = D3DCompileFromFile(
//			L"../MyLibrary/BasicHullShader.hlsl",
//			nullptr,
//			D3D_COMPILE_STANDARD_FILE_INCLUDE,
//			"HSmain", "hs_5_0",
//			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//			0,
//			&hsBlob, &errorBlob);
//
//		pDesc.HS.pShaderBytecode = hsBlob->GetBufferPointer();
//		pDesc.HS.BytecodeLength = hsBlob->GetBufferSize();*/
//#pragma endregion
//
//#pragma region ドメインシェーダー
//	/*	result = D3DCompileFromFile(
//			L"../MyLibrary/BasicDomainShader.hlsl",
//			nullptr,
//			D3D_COMPILE_STANDARD_FILE_INCLUDE,
//			"DSmain", "ds_5_0",
//			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//			0,
//			&dsBlob, &errorBlob);
//
//		pDesc.DS.pShaderBytecode = dsBlob->GetBufferPointer();
//		pDesc.DS.BytecodeLength = dsBlob->GetBufferSize();*/
//#pragma endregion
//
//	
//		
//	}
//
//	if (FAILED(result))
//	{
//		std::string errstr;
//		errstr.resize(errorBlob->GetBufferSize());
//
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			errstr.begin());
//		errstr += "\n";
//
//		OutputDebugStringA(errstr.c_str());
//		exit(1);
//	}
//
//#pragma endregion
//
//	//パイプラインの生成
//	result = dev->CreateGraphicsPipelineState(&pDesc, IID_PPV_ARGS(pipeline));
//
//	//パイプライン作成後に解放
//	if (useUserInputLayout)delete[] userInputLayout;
//
//	if (result != S_OK) 
//	{
//		OutputDebugString(L"パイプラインの生成に失敗しました\n");
//		return false;
//	}
//	return true;
//}
//
//
//void CreatePipeline::SetInputLayout(const std::vector<InputLayoutData>& inputLayoutData)
//{
//	//TYPELESS 1つ4バイトの型指定なし?
//	//FLOAT float
//	//UINT unsigned int
//	//SINT signed int(int)
//	
//
//	inputLayoutVector.reserve(inputLayoutData.size());
//	for (auto& ilData : inputLayoutData) 
//	{
//
//		if (ilData.number >= 5)return;
//
//		D3D12_INPUT_ELEMENT_DESC inputLayout;
//		DXGI_FORMAT dxgiFormat;
//
//		switch (ilData.formatType)
//		{
//		case FORMAT_TYPE::FLOAT:
//			switch (ilData.number)
//			{
//			case 1:
//				dxgiFormat = DXGI_FORMAT_R32_FLOAT;
//				break;
//			case 2:
//				dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
//				break;
//			case 3:
//				dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
//				break;
//			case 4:
//				dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
//				break;
//			}
//			break;
//
//		case FORMAT_TYPE::UNSIGNED_INT:
//			switch (ilData.number)
//			{
//			case 1:
//				dxgiFormat = DXGI_FORMAT_R32_UINT;
//				break;
//			case 2:
//				dxgiFormat = DXGI_FORMAT_R32G32_UINT;
//				break;
//			case 3:
//				dxgiFormat = DXGI_FORMAT_R32G32B32_UINT;
//				break;
//			case 4:
//				dxgiFormat = DXGI_FORMAT_R32G32B32A32_UINT;
//				break;
//			}
//			break;
//
//		case FORMAT_TYPE::SIGNED_INT:
//			switch (ilData.number)
//			{
//			case 1:
//				dxgiFormat = DXGI_FORMAT_R32_SINT;
//				break;
//			case 2:
//				dxgiFormat = DXGI_FORMAT_R32G32_SINT;
//				break;
//			case 3:
//				dxgiFormat = DXGI_FORMAT_R32G32B32_SINT;
//				break;
//			case 4:
//				dxgiFormat = DXGI_FORMAT_R32G32B32A32_SINT;
//				break;
//			}
//			break;
//		}
//
//		inputLayout =
//		{
//			ilData.semantics,
//			0,
//			dxgiFormat,
//			0,
//			D3D12_APPEND_ALIGNED_ELEMENT,
//			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//			0
//		};
//
//		inputLayoutVector.push_back(inputLayout);
//	}
//}
//
//void CreatePipeline::DeleteInputLayout() 
//{
//	inputLayoutVector.clear();
//	inputLayoutVector.shrink_to_fit();
//}
