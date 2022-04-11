#include "Sound.h"


using namespace MelLib;

IXAudio2* Sound::iXAudio2;

std::unordered_map<std::string, std::unique_ptr<Sound>> Sound::pSounds;
std::vector<std::unique_ptr<Sound>>Sound::pNoneNameSounds;
//
//class XAudio2VoiceCallback :public IXAudio2VoiceCallback
//{
//public:
//	HANDLE streamEndEvent;
//	XAudio2VoiceCallback() :streamEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)){}
//	~XAudio2VoiceCallback() { CloseHandle(streamEndEvent); }
//
//	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
//	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
//	
//	//指定したループ数分再生したら呼ばれる
//	STDMETHOD_(void, OnStreamEnd)(THIS) 
//	{
//		SetEvent(streamEndEvent); };
//	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
//	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext) {};
//	STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContrxt) {};
//	STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContrxt, HRESULT errow) {};
//
//};
//XAudio2VoiceCallback voiceCallback;

bool Sound::Play(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData)
{
	WAVEFORMATEX wfex = soundData->GetWaveFormatEX();

	auto result = iXAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if FAILED(result)
	{
#ifdef _DEBUG
		OutputDebugStringW(L"ソースボイスの生成に失敗しました。\n");
#endif // _DEBUG
		return false;
	}

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)soundData->GetPBuffer();
	buf.pContext = soundData->GetPBuffer();
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = soundData->GetBytes();
	buf.LoopCount = loopNum;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();


	SetPlaySoundData(playSoundData);

	pSoundData = soundData;
	this->loopNum = loopNum;

	return true;
}

void Sound::Initialize(IXAudio2* pIXAudio2)
{
	iXAudio2 = pIXAudio2;
}

void Sound::Update()
{

#pragma region 再生終了時の処理


	//std::vector<std::string>eraseSoundName;
	for(auto& s: pSounds)
	{
		s.second->playEndMoment = false;
		s.second->CheckPlayEnd();
		if(s.second->playEndMoment)
		{
			s.second->pSourceVoice->Stop();
			s.second->pSourceVoice->DestroyVoice();
			//eraseSoundName.push_back(s.first);
		}
	}
	//for(const auto& s : eraseSoundName)
	//{
	//	pSounds.erase(s);
	//}


	for (int i = 0, size = pNoneNameSounds.size(); i < size; i++)
	{
		pNoneNameSounds[i]->playEndMoment = false;
		pNoneNameSounds[i]->CheckPlayEnd();
		if(pNoneNameSounds[i]->playEndMoment)
		{
			pNoneNameSounds.erase(pNoneNameSounds.begin() + i);
			
			i--;
			size--;
		}
	}
#pragma endregion

}

void Sound::Finalize()
{
	pSounds.clear();
	pNoneNameSounds.clear();
}

void Sound::CheckPlayEnd()
{
	//WaitForSingleObjectEx(イベント,引数1のイベントがSetEventに渡されるまでに待機する時間,)
	DWORD res = WaitForSingleObjectEx(voiceCallback.streamEndEvent, 0, TRUE);

	if (res == WAIT_OBJECT_0)
	{
		playEndMoment = true;
		playEnd = true;
	}
	else
	{
		playEndMoment = false;
		playEnd = false;
	}
	
}

Sound::~Sound()
{
	if (pSourceVoice) 
	{
		pSourceVoice->Stop();
		pSourceVoice->DestroyVoice();
	}
}

bool Sound::PlayLoadSound(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData, const std::string& name)
{
	Sound* pSound = nullptr;
	if (name == "")
	{
		pNoneNameSounds.push_back(std::make_unique<Sound>());
		pSound = pNoneNameSounds[pNoneNameSounds.size() - 1].get();
	}
	else 
	{
		for(const auto& p : pSounds)
		{
			if(p.first == name)
			{
#ifdef _DEBUG
				OutputDebugStringA(name.c_str());
				OutputDebugStringW(L"は既に再生しています。\n");
#endif // _DEBUG
				return false;
			}
		}

		pSounds.emplace(name, std::make_unique<Sound>());
		pSound = pSounds[name].get();
	}

	
	if (!pSound->Play(soundData, loopNum, playSoundData))
	{
#ifdef _DEBUG
		OutputDebugStringA(name.c_str());
		OutputDebugStringW(L"の再生に失敗しました。\n");
#endif // _DEBUG
		return false;
	}

	return true;
}

void Sound::StopSound(const std::string& name)
{
	pSounds.erase(name);
	//pSounds[name]->pSourceVoice->Stop(XAUDIO2_PLAY_TAILS);
	//pSounds[name]->pSourceVoice->Stop(0);
}

void Sound::ResetSound()
{
	//ソースボイスを破棄して作り直す。
	if (pSourceVoice)
	{
		pSourceVoice->Stop();
		pSourceVoice->DestroyVoice();
		Play(pSoundData, loopNum, playSoundData);
	}



	//pSourceVoice->Stop(XAUDIO2_PLAY_TAILS);
	//pSourceVoice->Stop(0);
	//pSourceVoice->Start();
}

void Sound::SetPlaySoundData(const PlaySoundData& playSoundData)
{

	//PlaySoundDataの設定
	SetSoundVolume(playSoundData.volume);
}

void Sound::SetSoundVolume(const float volume)
{
	float volumePar = volume;
	if (volumePar > 100.0f)volumePar = 100.0f;
	else if (volumePar < 0.0f)volumePar = 0.0f;

	pSourceVoice->SetVolume(volumePar / 100.0f);
	playSoundData.volume = volume;
	
}

void Sound::SetSoundPauseFlag(const bool flag)
{
	if (flag)pSourceVoice->Stop();
	else pSourceVoice->Start();
}

Sound::XAudio2VoiceCallback::XAudio2VoiceCallback() :streamEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL))
{
}

Sound::XAudio2VoiceCallback::~XAudio2VoiceCallback()
{
	CloseHandle(streamEndEvent);
}

STDMETHODIMP_(void __stdcall) Sound::XAudio2VoiceCallback::OnStreamEnd(void)
{
	SetEvent(streamEndEvent);
}
