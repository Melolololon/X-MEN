#pragma once
#include<Windows.h>
#include<ctime>
#include<random>
#include"Vector.h"
#include"Color.h"
#include"DirectX12.h"
#include"DirectXStruct.h"
#include"CreatePolygon.h"


namespace MelLib
{

	class Library
	{


	private:
		Library() {}
		~Library() {}

		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		static DirectX12* dx12;
		//static CreatePolygon* createPolygon;

		static UINT count;
		static int createPointCount;


		static WNDCLASSEX w;
		static MSG msg;
		static HWND hwnd;
		static bool isEnd;


		static int loadFontTextureCounter;
		static int loadTextureCounter;
		static int createSpriteConter;

		static int createPipelineCounter;//現在のパイプライン生成数
		static int createPostEffectPuiperineCounter;//

		static HDC hdc;//dcのハンドル

		static bool isSetFPS60;
		static UINT fps;//現在設定されているfps
		static int refReat;//リフレッシュレート
		static DWORD startProsessTime;//計測開始時の時間
		static DWORD nowTime;//現在の時間

		//描画用(これにモデルデータ入れて、カメラから遠い順に並び替えて、これを拡張forで回して描画コマンドをセットすることで、
		//関数の呼び出し順にかかわらず、αブレンドが適切に行うことができる)
		static std::vector<std::tuple<ModelDataAndKey, int>>modelDatas;

#pragma region 初期化値
		static int winWidth;
		static int winHeight;
		static Color clearColor;
#pragma endregion
		static std::wstring winName;
	public:


#pragma region 初期化などの処理

		/// <summary>
		/// ライブラリの初期化を行います。どのライブラリの関数よりも先に呼び出してください
		/// </summary>
		/// <param name="windowWidth">ウィンドウ横幅</param>
		/// <param name="windowHeight">ウィンドウ縦幅</param>
		static void Initialize(int windowWidth, int windowHeight, const Color& screenColor, const wchar_t* winName);

		/// <summary>
		/// 描画準備をします。更新処理部分の一番上で呼び出してください
		/// </summary>
		static void LoopStartProcess();

		/// <summary>
		/// 描画します。描画処理の一番最後の呼び出してください
		/// </summary>
		static void LoopEndProcess();

		/// <summary>
		/// ライブラリの終了フラグをtrueにします
		/// </summary>
		static void EndFlagTrue();

		/// <summary>
		/// 終了フラグを取得します。終了フラグは、ウィンドウが破棄される(×が押される)か、EndFlagTrueよ呼び出すとtrueになります。
		/// </summary>
		/// <returns>終了フラグ</returns>
		static bool GetIsEnd();

		/// <summary>
		/// 終了処理を行います
		/// </summary>
		static void Finalize();
#pragma endregion

#pragma region 初期化値取得
		static int GetWindowWidth() { return winWidth; }
		static int GetWindowHeight() { return winHeight; }
		static Color GetScreenColor() { return clearColor; }
#pragma endregion

#pragma region ウィンドウ関係
		static HWND GetHWND(){ return hwnd; }
		static std::wstring GetWindowName() { return winName; }
#pragma endregion




#pragma region 設定

		/// <summary>
		/// FPSを60に固定します
		/// </summary>
		/// <param name="framesPerSecond">フラグ</param>
		static void SetFramesPerSecond60(bool flag);



#pragma endregion


	};

}
