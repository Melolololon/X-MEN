#include "Library.h"
#include"LibMath.h"

#include"Input.h"

#pragma comment(lib,"winmm.lib")//timeGetTimeと
#include"GameObjectManager.h"
#include"SceneManager.h"

#include"Audio.h"
#include"Sound.h"

#include"Random.h"
#include"FrameTimer.h"

#include"LibWinAPI.h"
#include"TextWrite.h"

#include"Material.h"
#include"SpriteFont2D.h"


using namespace MelLib;

DirectX12* Library::dx12;
//CreatePolygon* Library::createPolygon;

WNDCLASSEX Library::w;
MSG  Library::msg;
HWND Library::hwnd;
bool Library::isEnd;

UINT Library::count;

int Library::createPointCount;

bool Library::isSetFPS60;
UINT Library::fps;

int Library::loadFontTextureCounter;
int Library::loadTextureCounter;
int Library::createSpriteConter;

int Library::createPipelineCounter;
int Library::createPostEffectPuiperineCounter;

HDC Library::hdc;
int Library::refReat;
DWORD Library::startProsessTime;
DWORD Library::nowTime;

std::vector<std::tuple<ModelDataAndKey, int>> Library::modelDatas;

int Library::winWidth;
int Library::winHeight;
Color Library::clearColor;
std::wstring Library::winName;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);


//利用者がウィンドウを自作して使う場合は、利用者が用意したProcでこのProcを呼び出してもらえば、
//ImGuiの処理を行える
LRESULT Library::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void Library::Initialize(int windowWidth, int windowHeight, const Color& screenColor, const wchar_t* windowName)
{
	 winWidth = windowWidth;
	 winHeight = windowHeight;
	 clearColor = screenColor;
 	 winName = windowName;

	auto coResult = CoInitialize(NULL);

	dx12 = DirectX12::GetInstance();
	/*createPolygon = CreatePolygon::GetInstance();
	createPolygon->Initialize(windowWidth, windowHeight);*/

	srand((unsigned int)time(NULL));
	Random::Initialize();
	GameObjectManager::GetInstance()->Initialize();

	count = 0;
	fps = 60;
	startProsessTime = timeGetTime();
	nowTime = timeGetTime();

#pragma region ウィンドウ処理

	hwnd = LibWinAPI::CreateNormalWindow
	(
		windowName,
		windowName,
		WS_OVERLAPPEDWINDOW /*| WS_MAXIMIZE *//*| WS_CLIPCHILDREN*/,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowWidth,
		windowHeight,
		nullptr,
		WindowProc
	);
#pragma endregion

#pragma region DC

	//DCを取得
	hdc = GetDC(hwnd);
	//リフレッシュレートを取得
	refReat = GetDeviceCaps(hdc, VREFRESH);
#pragma endregion

	

	dx12->SetScreenColor(screenColor);
	dx12->Initialize(hwnd, windowWidth, windowHeight);
	


	Input::Initialize(hwnd, windowWidth, windowHeight);
	Audio::GetInstance()->Initialize();
	SpriteFont2D::GetInstance()->Initialize();

#ifdef _DEBUG


	GameObject::CreateCollisionCheckModelPipelineState();
#endif // _DEBUG
}

void Library::LoopStartProcess()
{
	startProsessTime = timeGetTime();

#pragma region ウィンドウ処理

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
	{
		isEnd = true;
	}
	

#pragma endregion


	dx12->LoopStartProcess();

	Input::Update();
	Sound::Update();
	SpriteFont2D::GetInstance()->Update();
	FrameTimer::AllUpdate();
}

void Library::LoopEndProcess()
{
	dx12->LoopEndProcess();
	
	if (isSetFPS60)
	{

		//60fpsの時の1フレームの処理時間を計測(この数値は、環境に依存しない)
		//これ毎回計測する必要ない。fps変えた時に変更すればいい
		float pureTime = 1000.0f / (float)fps * ((float)count - 1.0f);
		float currTime = 1000.0f / (float)fps * (float)count;
		int taikiTime = (int)currTime - (int)pureTime;//待機しないといけない時間
		count++;
		count = count >= fps ? 0 : count;

		//前ループからどのくらい時間が経過したかを計測(preを、startProsessのほうに移し、GetTickCount64入れる)
		//previousTime = currentTime;
		nowTime = timeGetTime();
		DWORD keika = nowTime - startProsessTime;
		//sabun /= 1000;

		//待機時間を計測(taikiTimeとsabunの差だけsleeで待機)
		int sleepTime = taikiTime - keika;
		sleepTime = sleepTime <= 0 ? 0 : sleepTime;

		//スリープ
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
	}



}

void Library::EndFlagTrue()
{
	isEnd = true;
}

bool Library::GetIsEnd()
{
	return isEnd;
}

void Library::Finalize()
{
	if (hwnd) DestroyWindow(hwnd);
	//LoopEndProcess呼んでから終了するので、dx12を先にリリースしないとモデルのバッファが消えて終了処理上手く実行できなくなる
	dx12->Finalize();

	SceneManager::GetInstance()->Finalize();
	GameObjectManager::GetInstance()->Finalize();
	Input::Finalize();

	Sound::Finalize();
	SoundData::Finalize();

	UnregisterClass(w.lpszClassName, w.hInstance);

	CoUninitialize();
}


#pragma region 設定

void Library::SetFramesPerSecond60(bool flag)
{
	isSetFPS60 = flag;
}
//
//void Library::setScreenColor(Color color)
//{
//	dx12->setScreenColor(color);
//}

#pragma endregion


