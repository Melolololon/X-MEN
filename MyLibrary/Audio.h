#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<x3daudio.h>

#include<wrl.h>
#include<inttypes.h>
using namespace Microsoft::WRL;

namespace MelLib 
{
	//オーディオクラス(XAudio2とX3DAudio管理クラス)
	class Audio
	{
	private:
		Audio() {}
		~Audio() {}

		//XAudio2
		ComPtr<IXAudio2>xAudio2;
		//マスターボイス(IXAudio2と同時に自動的に解放される)
		IXAudio2MasteringVoice* masterVoice = nullptr;

		

	public:
		Audio(const Audio& a) = delete;
		Audio& operator=(const Audio& a) = delete;
		static Audio* GetInstance();

		void Initialize();

	};

}