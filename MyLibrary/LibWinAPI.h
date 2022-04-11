#pragma once
#include<Windows.h>
#include<string>
#include"Vector.h"

//L"EDIT"とかじゃない子ウィンドウは、親の名前渡さないといけない?

namespace MelLib
{
	//WinAPIをまとめたもの
	class LibWinAPI
	{
	private:
		//static WNDCLASSEX w;


	public:

		/// <summary>
		/// ウィンドウを作成します。
		/// </summary>
		/// <param name="className">ウィンドウクラス名</param>
		/// <param name="windowName">ウィンドウ名(ボタンなどを作成する場合、書かれている名前)</param>
		/// <param name="windowStyle">ウィンドウのスタイル</param>
		/// <param name="posX">縦幅</param>
		/// <param name="posY">縦幅</param>
		/// <param name="windowWidth">縦幅</param>
		/// <param name="windowHeighr">横幅</param>
		/// <param name="parentHWND">親ウィンドウのHWND。親ウィンドウを生成する場合は、nullptrを渡す。</param>
		/// <param name="winProc">ウィンドウプロシージャ</param>
		/// <returns></returns>
		static HWND CreateNormalWindow
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
		);

		static HWND CreateExpansionWindow
		(
			const std::wstring& className,
			const std::wstring& winName,
			const DWORD& expansionWindowStyle,
			const DWORD& windowStyle,
			const DWORD& posX,
			const DWORD& posY,
			const int& width,
			const int& height,
			const HWND parentHWND,
			const WNDPROC& winProc
		);

		//static HWND createExpansionWindow
		//(
		//	const std::wstring& windowName,
		//	const int& width,
		//	const int& heighr,
		//	const int& windowStyle,
		//	const HWND parentHWND,
		//	const WNDPROC& winProc
		//);

		/// <summary>
		/// 保存するためのウィンドウを生成し、入力したファイル名を取得します
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="fileName">ファイル名を入力する部分の初期名を記述する。trueが返された場合、入力したファイル名が格納される。</param>
		/// <param name="defaultFormat">フォーマットを指定しなかった場合のフォーマット名</param>
		/// <returns></returns>
		static bool CreateSaveWindow
		(
			const HWND& hwnd,
			std::wstring& fileName,
			const std::wstring& defaultFormat
		);

		/// <summary>
		///	開くためのウィンドウを生成し、入力したファイル名を取得します
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="fileName">ファイル名を入力する部分の初期名を記述する。trueが返された場合、入力したファイル名が格納される。</param>
		/// <param name="defaultFormat">フォーマットを指定しなかった場合のフォーマット名</param>
		/// <returns></returns>
		static bool CreateLoadWindow
		(
			const HWND& hwnd,
			std::wstring& fileName,
			const std::wstring& defaultFormat
		);

	};
}
