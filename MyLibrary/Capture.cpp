#include "Capture.h"

MelLib::ScreenCapture* MelLib::ScreenCapture::GetInstance()
{
	static ScreenCapture s;
	return &s;
}

void MelLib::ScreenCapture::GetCaptureData(Texture& refTexture, HWND hwnd)
{
	// Windows GDIで読み込み



}

void MelLib::ScreenCapture::GetCaptureData(Texture& refTexture, const std::wstring& windowName)
{
	// 指定したウィンドウのハンドルを取得
	HWND hwnd = FindWindow(NULL, windowName.c_str());

	HDC hdcWindow = GetDC(hwnd);

	BITMAP bmp;
	GetObject(hdcWindow, sizeof(BITMAP), &bmp);

	// おそらく画像部分のサイズを指定すればいいんだろうけど、チュートリアルでわからない数字があってよくわからん
	// もしかするとバイトじゃなくてビットで指定するとか?
	size_t bmpDataSize = (bmp.bmWidth * bmp.bmHeight) * 4;

	// GHNDは可動メモリを割り当て、更に0で初期化
	// 画像データはサイズによって必要メモリが違うからGHNDにしてる
	//HANDLE hDIB = GlobalAlloc(GHND,);

	// GetDIBitsでおそらく画像情報取得できる

	BITMAPINFOHEADER header;
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biWidth = bmp.bmWidth;
	header.biHeight = bmp.bmHeight;
}

void MelLib::VideoCapture::StartCapture(HWND hwnd)
{
}

void MelLib::VideoCapture::EndCapture()
{
}

void MelLib::VideoCapture::GetCurrentFrameCaptureTexture(Texture& refTexture)const 
{
}

void MelLib::VideoCapture::Update()
{
}

MelLib::VideoCaptureManager* MelLib::VideoCaptureManager::GetInstance()
{
	static VideoCaptureManager v;
	return &v;
}

void MelLib::VideoCaptureManager::Update()
{
}
