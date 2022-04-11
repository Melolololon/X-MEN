#include "LibWinAPI.h"

//子ウィンドウは、WNDCLASSEXを登録しちゃいけない。登録するとボタンとか作れない

//子ウィンドウ作るときはproc渡さないようにする

//これメンバ変数にしないとHWNDがNULLになる
//WNDCLASSEX LibWinAPI::w;
using namespace MelLib;

HWND LibWinAPI::CreateNormalWindow
(
	const std::wstring& className,
	const std::wstring& winName,
	const DWORD& windowStyle,
	const DWORD& posX,
	const DWORD& posY,
	const int& width,
	const int& height,
	const HWND parentHWND,
	const WNDPROC& winProc
)
{
	WNDCLASSEX w;
	if (!parentHWND)
	{
		w = {};
		w.cbSize = sizeof(WNDCLASSEX);
		w.lpfnWndProc = (WNDPROC)winProc;
		w.lpszClassName = className.c_str();
		w.hInstance = GetModuleHandle(nullptr);
		w.hCursor = LoadCursor(NULL, IDC_ARROW);

		RegisterClassEx(&w);
	}
	RECT wrc = { 0,0,width,height };
	AdjustWindowRect(&wrc, windowStyle, false);


	HWND hwnd;
	hwnd = CreateWindow
	(
		className.c_str(),
		winName.c_str(),
		windowStyle,
		posX,
		posY,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		parentHWND,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);

	ShowWindow(hwnd, SW_SHOW);

	return hwnd;
}

HWND LibWinAPI::CreateExpansionWindow(const std::wstring& className, const std::wstring& winName, const DWORD& expansionWindowStyle, const DWORD& windowStyle, const DWORD& posX, const DWORD& posY, const int& width, const int& height, const HWND parentHWND, const WNDPROC& winProc)
{
	WNDCLASSEX w;
	if (!parentHWND)
	{
		w = {};
		w.cbSize = sizeof(WNDCLASSEX);
		w.lpfnWndProc = (WNDPROC)winProc;
		w.lpszClassName = className.c_str();
		w.hInstance = GetModuleHandle(nullptr);
		w.hCursor = LoadCursor(NULL, IDC_ARROW);

		RegisterClassEx(&w);
	}
	RECT wrc = { 0,0,width,height };
	AdjustWindowRectEx(&wrc, windowStyle, false, expansionWindowStyle);


	HWND hwnd;
	hwnd = CreateWindowEx
	(
		expansionWindowStyle,
		className.c_str(),
		winName.c_str(),
		windowStyle,
		posX,
		posY,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		parentHWND,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);

	ShowWindow(hwnd, SW_SHOW);

	return hwnd;
}



//
//HWND LibWinAPI::createExpansionWindow
//(
//	const std::wstring& windowName,
//	const int& width,
//	const int& heighr,
//	const int& windowStyle,
//	const HWND parentHWND,
//	const WNDPROC& winProc
//)
//{
//	w.cbSize = sizeof(WNDCLASSEX);
//	w.lpfnWndProc = (WNDPROC)winProc;
//	w.lpszClassName = windowName.c_str();
//	w.hInstance = GetModuleHandle(nullptr);
//	w.hCursor = LoadCursor(NULL, IDC_ARROW);
//
//	RegisterClassEx(&w);
//	RECT wrc = { 0,0,width,height };
//	AdjustWindowRect(&wrc, windowStyle, false);
//
//	HWND hwnd;
//	hwnd = CreateDirectoryEx
//	(
//		w.lpszClassName,
//		windowName.c_str(),
//		windowStyle,
//		CW_USEDEFAULT,
//		CW_USEDEFAULT,
//		wrc.right - wrc.left,
//		wrc.bottom - wrc.top,
//		parentHWND,
//		nullptr,
//		w.hInstance,
//		nullptr);
//
//	ShowWindow(hwnd, SW_SHOW);
//
//	return hwnd;
//}


bool LibWinAPI::CreateSaveWindow
(
	const HWND& hwnd, 
	std::wstring& fileName,
	const std::wstring & defaultFormat
)
{
	OPENFILENAMEW save;
	//初期状態のときの表示
	fileName.resize(400);

	save.lStructSize = sizeof(OPENFILENAME);
	save.hwndOwner = hwnd;//ここにHWNDをセットするとダイアログのハンドルが格納される?
	save.hInstance = GetModuleHandle(nullptr);
	save.lpstrFilter = nullptr;
	save.lpstrCustomFilter = nullptr;
	save.nMaxCustFilter = 0;
	save.nFilterIndex = 1;
	save.lpstrFile = (LPTSTR)fileName.data();//ここに入力したものが保存される
	save.nMaxFile = fileName.size();
	save.lpstrFileTitle = NULL;//NULLにすると、パスがC:から始まる
	save.nMaxFileTitle = 0;
	save.lpstrInitialDir = (LPCWSTR)L"C:";//ウィンドウ開いて最初に表示する場所?違うかも
	save.lpstrTitle = NULL;
	save.Flags = OFN_HIDEREADONLY;
	save.nFileOffset = 0;
	save.nFileExtension = 8;
	save.lpstrDefExt = (LPWSTR)defaultFormat.c_str();//形式を入力しなかった場合、何の形式にするか
	//saveName.lCustData = ram;

	bool result = GetSaveFileName(&save);//ウィンドウハンドル取得してプロシージャでメッセージ取得して、破棄したらGamePlayのisSaveをfalseに変えるようにする

	if (!result)return result;
	int strCount = 0;
	for(auto& wStr : fileName)
	{
		if(wStr == '\0')
		{
			fileName.erase(fileName.begin() + strCount, fileName.end());
			break;
		}
		strCount++;
	}
	return result;

}

bool LibWinAPI::CreateLoadWindow
(
	const HWND& hwnd,
	std::wstring& fileName,
	const std::wstring& defaultFormat
)
{
	OPENFILENAMEW load;
	//初期状態のときの表示
	fileName.resize(400);

	load.lStructSize = sizeof(OPENFILENAME);
	load.hwndOwner = hwnd;//ここにHWNDをセットするとダイアログのハンドルが格納される?
	load.hInstance = GetModuleHandle(nullptr);
	load.lpstrFilter = nullptr;
	load.lpstrCustomFilter = nullptr;
	load.nMaxCustFilter = 0;
	load.nFilterIndex = 1;
	load.lpstrFile = (LPTSTR)fileName.data();//ここに入力したものが保存される
	load.nMaxFile = fileName.size();
	load.lpstrFileTitle = NULL;//NULLにすると、パスがC:から始まる
	load.nMaxFileTitle = 0;
	load.lpstrInitialDir = (LPCWSTR)L"C:";//ウィンドウ開いて最初に表示する場所?違うかも
	load.lpstrTitle = NULL;
	load.Flags = OFN_HIDEREADONLY;
	load.nFileOffset = 0;
	load.nFileExtension = 8;
	load.lpstrDefExt = (LPWSTR)defaultFormat.c_str();//形式を入力しなかった場合、何の形式にするか
	//saveName.lCustData = ram;

	bool result = GetOpenFileName(&load);//ウィンドウハンドル取得してプロシージャでメッセージ取得して、破棄したらGamePlayのisSaveをfalseに変えるようにする

	if (!result)return result;
	int strCount = 0;
	for (auto& wStr : fileName)
	{
		if (wStr == '\0')
		{
			fileName.erase(fileName.begin() + strCount, fileName.end());
			break;
		}
		strCount++;
	}

	return result;
}
