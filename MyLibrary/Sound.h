#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<wrl.h>
#include<inttypes.h>
#include<string>
#include<unordered_map>
#include"SoundData.h"
using namespace Microsoft::WRL;


namespace MelLib
{
	//サウンドのパラメータ
	struct PlaySoundData
	{
		//ボリューム(パーセント)
		float volume = 50.0f;

	};

	//サブミックスボイスでソースボイスをまとめて管理できるらしい
	//「方法：ソースボイスコールバックを使用する」にコールバックが呼ばれたときにソースボイス解放したりする処理書いてある?
	//https://docs.microsoft.com/en-us/windows/win32/xaudio2/how-to--use-source-voice-callbacks

	//ほぼすべてのSEを調整したいときとかはどうするか。
	//PlayLoadSoundでどのサブミックスボイスに追加するかを選べるようにする?
	//サブミックスボイスも作れるようにする?サブミックスボイスだと分かりにくいかもしれないからサウンドグループとかにする?

	//サウンドもModelObjectとかSprite2Dみたいに自分で管理できるようにする?

	//ソースボイス消さなくても初めから再生し直せるっぽい

	//SoundDataの音声を再生したり加工するクラス
	class Sound
	{
	private:
		//コールバック
		class XAudio2VoiceCallback :public IXAudio2VoiceCallback
		{
		public:
			HANDLE streamEndEvent;
			XAudio2VoiceCallback();
			~XAudio2VoiceCallback();

			STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
			STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};

			//指定したループ数分再生したら呼ばれる
			STDMETHOD_(void, OnStreamEnd)(THIS);
			STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
			STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext) {};
			STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContrxt) {};
			STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContrxt, HRESULT errow) {};

		};
		XAudio2VoiceCallback voiceCallback;

		static std::unordered_map<std::string, std::unique_ptr<Sound>> pSounds;
		static std::vector<std::unique_ptr<Sound>>pNoneNameSounds;

		static IXAudio2* iXAudio2;
		IXAudio2SourceVoice* pSourceVoice;

		SoundData* pSoundData;
		PlaySoundData playSoundData;

		//ループ数
		UINT32 loopNum = 0;
		//ループ回数
		UINT loopCount = 0;

		//再生が終了した瞬間かどうか
		bool playEndMoment = false;
		//再生が終了したかどうか
		bool playEnd = false;


		bool Play(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData);
		void CheckPlayEnd();
	public:
		virtual ~Sound();

		static const UINT32 LOOP_INFINITY = XAUDIO2_LOOP_INFINITE;
		static const UINT32 LOOP_MAX = XAUDIO2_MAX_LOOP_COUNT;

		static void Initialize(IXAudio2* pIXAudio2);
		static void Update();
		static void Finalize();


		/// <summary>
		/// SoundDataを再生します。引数nameに名前を渡すとGet関数で参照できるようになり、音声のデータを後から設定したり取得したりすることができます。
		/// nameに引数を渡した場合は再生終了後にメモリが解放されないため、StopSound関数を呼び出し、手動で解放してください。
		/// </summary>
		/// <param name="soundData">音声データ</param>
		/// <param name="loopNum">ループ数(0から254(LOOP_MAX)またはLOOP_INFINITY)</param>
		/// <param name="playSoundData">サウンドの設定をまとめたもの</param>
		/// <param name="name">登録名</param>
		/// <returns></returns>
		static bool PlayLoadSound(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData, const std::string& name = "");

		static Sound* Get(const std::string& name) { return pSounds[name].get(); }

		/// <summary>
		/// 再生を終了します。Get関数での参照が不可能になるので注意してください。
		/// </summary>
		/// <param name="name"></param>
		static void StopSound(const std::string& name);

		/// <summary>
		/// 最初から再生しなおします。
		/// </summary>
		/// <param name="name"></param>
		void ResetSound();



#pragma region セット

		/// <summary>
		/// playSoundDataのデータを全てセットします。
		/// </summary>
		/// <param name="playSoundData"></param>
		void SetPlaySoundData(const PlaySoundData& playSoundData);

		/// <summary>
		/// 音量を設定します。
		/// </summary>
		/// <param name="volume">音量(0から100のパーセント)</param>
		void SetSoundVolume(const float volume);

		/// <summary>
		/// 一時停止フラグをセットします。
		/// </summary>
		/// <param name="flag"></param>
		void SetSoundPauseFlag(const bool flag);


#pragma endregion

#pragma region ゲット
		/// <summary>
		/// 再生が終了した瞬間にtrueを返します。
		/// </summary>
		/// <returns></returns>
		bool GetPlayEndMomentFlag() { return playEndMoment; }

		/// <summary>
		/// 再生が終了していたらtrueを返します。
		/// </summary>
		/// <returns></returns>
		bool GetPlayEnd() { return playEnd; }

		UINT GetLoopNumber() { return loopNum; }

		UINT GetLoopCount() { return loopCount; }
#pragma endregion

	};

}