#pragma once
#include<Windows.h>
#include<vector>
#include<cmath>
#include<unordered_map>
#include<random>
#include<ctime>
#include<string>
#include<unordered_map>
#include<wrl.h>


#include"DirectInput.h"
#include"CreateBuffer.h"
#include"CreatePolygon.h"
#include"CreatePipeline.h"
#include"ModelLoader.h"
#include"CreateCamera.h"
#include"Vector.h"
#include"DirectXStruct.h"
#include"DirectXTexLoader.h"

#define DIRECTINPUT_VERSION 0x0800
#include<d3d12.h>
#include<dxgi1_6.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#include<dxgidebug.h>
#include<dinput.h>
#include<DirectXTex.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include"Sprite.h"
#include"Sprite2DBase.h"
#include"Sprite3D.h"

//d3d12のサンプル一覧
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/working-samples


using namespace Microsoft::WRL;
namespace MelLib
{

	enum LibraryPipeline
	{
		PIPELINE_NOT_DEPTH_TEST,//深度テスト無し(zにかかわらず、関数の呼び出した順番で描画されます)
		PIPELINE_CULL_NONE,//カリングなし(モデルの裏側も描画します)
		PIPELINE_NO_WRITE_ALPHA,//透明部分を書き込まない
		PIPELINE_NORMAL,
		PIPELINE_OBJ_ANIMATION,
	};

	//DirectX12関係の管理クラス
	class DirectX12 final
	{

		enum DrawType
		{
			none,
			box,
			circle,
		};

		enum Dimension
		{
			dimention2D,
			dimention3D
		};

	private:
		
		float clearColor[4] = { 0,0,0,1 };

#pragma region 現在の描画コマンド状態
		enum RSState
		{
			RS_MODEL,
			RS_SPRITE,
			RS_POINT,
		};
#pragma endregion


#pragma region Windows
		HWND hwnd;
		int winWidth = 0;
		int winHeight = 0;
#pragma endregion

#pragma region 基本的なオブジェクト

		//DirectX変数
		HRESULT result = S_OK;
		ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;
		ComPtr<ID3D12Device> dev = nullptr;
		ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
		ComPtr<IDXGISwapChain4> swapchain = nullptr;
		ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
		ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;


#pragma region RTV

		//メイン
		ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;
		ComPtr<ID3D12Resource> backBuffer[2] = { nullptr ,nullptr };
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

#pragma endregion


		ComPtr<ID3D12DescriptorHeap> depthHeap = nullptr;
		DepthBufferSet depthBufferSet;

		ComPtr<ID3D12Fence> fence = nullptr;
		UINT64 fenceVal = 0;

		D3D12_RESOURCE_BARRIER barrierDesc = {};


#pragma endregion

		DirectX12();
		~DirectX12();

	public:

		DirectX12(DirectX12& d) = delete;
		DirectX12 operator=(DirectX12& d) = delete;
		static DirectX12* GetInstance();

#pragma region 初期化などの必須処理

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(HWND hwnd, int windouWidth, int windowHeight);

		/// <summary>
		/// 描画コマンドなどを呼び出す前に呼ぶ処理
		/// </summary>
		void LoopStartProcess();
		//描画処理。コマンドリストの命令を実行したりする
		void LoopEndProcess();

		void RenderingBackBaffer();

		void Finalize();
#pragma endregion


		//画面の色受け取り
		void SetScreenColor(Color screenColor);

	};

}