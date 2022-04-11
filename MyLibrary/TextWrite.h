#pragma once
#include<d2d1.h>
#pragma comment(lib,"D2d1.lib")
#include<d2d1_3.h>
#include<dwrite.h>
#pragma comment(lib,"DWrite.lib")

#pragma once
#include<d3d12.h>
#include<d3dx12.h>
#include<d3d11.h>
#include<d3d11on12.h>
#pragma comment(lib,"d3d11.lib")
#include<dxgi.h>
#pragma comment(lib,"dxgi.lib")

#include<wrl.h>
#include<string>
#include<unordered_map>

#include"Vector.h"
#include"Color.h"



//DirectWriteチュートリアルURL
//https://docs.microsoft.com/ja-jp/windows/win32/directwrite/getting-started-with-directwrite#part-1-declare-directwrite-and-direct2d-resources

//pTextFormatは別クラスにする?
//フォントクラスとテキスト描画クラスを分ける?
//とりあえず描画までやってみてから設計考える

//D3D11On12を使わないと無理っぽい
//使用例のところにやり方書いてる
//https://docs.microsoft.com/en-us/windows/win32/direct3d12/direct3d-11-on-12#initializing-d3d11on12


//11on12を使用したD2D
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/d2d-using-d3d11on12

using namespace Microsoft::WRL;

namespace MelLib
{
	//DirectWriteを使用したテキスト描画クラス
	class TextWrite
	{
	public:

		//enum class TextAlignment
		//{
		//	LEFT_JUSTIFIED,//左揃え
		//	RIGHT_JUSTIFIED,//右揃え
		//	POINT//中心揃え
		//};
		struct FontName
		{
			const std::wstring ARIAL = L"Arial";
			const std::wstring HGP_GOSIC_E = L"HGPｺﾞｼｯｸE";
		};


	private:
		static std::vector<std::wstring>tests;
		static std::vector<std::string>fontNames;
		static std::vector<std::tuple<Vector2, Color,/* TextAlignment,*/std::wstring, std::string>>drawTextDatas;

		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


		//DirectWriteを使用するためのもの
		static ComPtr<IDWriteFactory> dWriteFactory;
		static std::unordered_map<std::string, ComPtr<IDWriteTextFormat>>pTextFormat;

		//11のデバイス
		static ComPtr<ID3D11Device>d3d11Device;
		static ComPtr<ID3D11DeviceContext>d3d11context;
		static ComPtr<ID3D11On12Device>d3d11On12device;
		//レンダリングするバッファ
		static ComPtr<ID3D11Resource>wrappedBackBuffer[2];

		//D2D
		static ComPtr<ID2D1Factory3> d2dFactory;
		static ComPtr<ID2D1Device2>d2dDevice;
		static ComPtr<IDXGIDevice>dxgiDevice;
		static ComPtr<ID2D1DeviceContext> d2dContext;

		//バインドされているビットマップを表す
		static ComPtr<ID2D1Bitmap1>d2dRenderTerget[2];

		//レンダリングするためのもの
		static ComPtr<ID2D1HwndRenderTarget> d2dRenderTarget;
		//領域を塗りつぶすもの(もしかしてフォントの塗りつぶしに使う?)
		static ComPtr<ID2D1SolidColorBrush> d2dSolidColorBrush;

		static HWND hwnd;


	public:
		static bool Initialize
		(
			ID3D12Device* pD3D12Device,
			ID3D12CommandQueue** pPD3D12Queue,
			ID3D12Resource* pBuckBuffer[2]
		);

		static void LoopStartProcess();
		static void LoopEndProcess(const UINT rtIndex);

		//フォントのデータをセットする関数作る?
		static bool CreateFontData(const std::wstring& fontName,float size, const std::string& name);


		//文字サイズは行列設定できるからそれで変えるようにする?

		static void Draw
		(
			const Vector2& position,
			const Color& color,
			/*	const TextAlignment allgnment,*/
			const std::wstring& text,
			const std::string& fontName
		);
	};
}


