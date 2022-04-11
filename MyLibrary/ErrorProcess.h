#pragma once
#include<string>
#include<Windows.h>
//エラー時にウィンドウを出し、処理するクラス
namespace MelLib 
{
	class ErrorProcess
	{
	private:
		ErrorProcess() {}
		~ErrorProcess() {}

		HWND errowMessageHWND = nullptr;
	public:
		ErrorProcess(ErrorProcess& e) = delete;
		ErrorProcess operator=(ErrorProcess& e) = delete;
		static ErrorProcess* GetInstance();

		/// <summary>
		/// 呼び出すとエラーが起きた時の処理を行います。
		/// </summary>
		/// <param name="text">エラーメッセージ</param>
		/// <param name="assertFlag"></param>
		void StartErroeProcess(const std::wstring& text,const bool assertFlag);
	};
}

