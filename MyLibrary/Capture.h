#pragma once
#include<vector>
#include<unordered_map>
#include<string>
#include<Windows.h>

#include"Library.h"
#include"Texture.h"

namespace MelLib 
{
	// レンダーターゲットも指定できるようにしちゃおう

	// 画面をキャプチャするクラス
	class ScreenCapture final
	{
	private:
		ScreenCapture(){}
		~ScreenCapture(){}
	public:
		ScreenCapture(ScreenCapture& s) = delete;
		ScreenCapture& operator=(ScreenCapture& s) = delete;
		static ScreenCapture* GetInstance();

		/// <summary>
		/// キャプチャした結果をTexture型で受け取ります。
		/// </summary>
		/// <param name="texture"></param>
		/// <param name="hwnd">キャプチャ対象。指定しない場合、ライブラリで生成したウィンドウが指定される</param>
		void GetCaptureData(Texture& refTexture,HWND hwnd = Library::GetHWND());

		/// <summary>
		/// キャプチャした結果をTexture型で受け取ります。
		/// </summary>
		/// <param name="refTexture"></param>
		/// <param name="windowName">キャプチャしたいウィンドウの名前</param>
		void GetCaptureData(Texture& refTexture, const std::wstring& windowName);
	};


	// 複数キャプチャできるようにするためにシングルトンじゃなくした
	// シーンをまたいで録画する可能性があるため、録画の開始終了自体はシングルトンで行ったほうが良い

	// ウィンドウを録画するクラス
	class VideoCapture
	{
	private:
		// 録画したデータ
		std::vector<std::vector<std::vector<Texture>>>captureData;

		HWND hwnd = nullptr;
	public:

		/// <summary>
		/// キャプチャを開始します
		/// </summary>
		/// <param name="refCaptureAddVector"></param>
		/// <param name="hwnd"></param>
		void StartCapture(HWND hwnd = nullptr);

		void EndCapture();

		/// <summary>
		/// 現在の画面のフレームのキャプチャしたテクスチャ
		/// </summary>
		/// <param name="refTexture"></param>
		void GetCurrentFrameCaptureTexture(Texture& refTexture)const;


		/// <summary>
		/// 自動で呼ばれるため、呼び出す必要なし
		/// </summary>
		void Update();
	};

	// 録画を管理するクラス
	class VideoCaptureManager final 
	{
	private:

		

	private:
		// 登録名,キャプチャ
		std::unordered_map<std::string,VideoCapture*>pVideoCaptures;

	private:
		VideoCaptureManager() {}
		~VideoCaptureManager() {}
	public:
		VideoCaptureManager(VideoCaptureManager& v) = delete;
		VideoCaptureManager& operator=(VideoCaptureManager& v) = delete;
		static VideoCaptureManager* GetInstance();

		void StartCapture(const std::string& name);
		void EndCapture(const std::string& name);
		const VideoCapture& GetCapture(const std::string& name) { return *pVideoCaptures.at(name); }

		void Update();
	};
}

