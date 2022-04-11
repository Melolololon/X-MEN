//#pragma once
//#include<xaudio2.h>
//#pragma comment(lib,"xaudio2.lib")
//#include<wrl.h>
//#include<inttypes.h>
//#include<unordered_map>
//using namespace Microsoft::WRL;
//
//struct WAVChunk
//{
//	char id[4];
//	int32_t size;
//};
//
//struct WAVRiffHeader
//{
//	WAVChunk chunk;
//	char type[4];
//};
//
//struct WAVFormatChunk
//{
//	WAVChunk chunk;
//	WAVEFORMAT fmt;
//};
//
////ソースボイスは定数バッファ、XAUDIO2_BUFFERは頂点バッファ的な考えでいい?
////XAUDIO2_BUFFER使いまわしてソースボイス複数用意すれば複数再生できる?無理?
////再生位置リセットないけど、もしかしてソースボイス消す前提?Stop関数はバッファ削除するためのある?でもStopにリセットっぽい引数のあったような
//
//
////サウンドクラス
//class Sound
//{
//private:
//
//	static ComPtr<IXAudio2>xAudio2;
//	//マスターボイス(IXAudio2と同時に自動的に解放される)
//	static IXAudio2MasteringVoice* masterVoice;
//
//	IXAudio2SourceVoice* spurceVoice;
//
//	//サウンドのu_map
//	std::unordered_map<std::string, IXAudio2SourceVoice*> pLoadSourceVoices;
//
//
//	static std::unordered_map<std::string, std::unique_ptr<Sound>> pSound;
//	char* pBuffer = nullptr;
//
//
//	static std::unordered_map<std::string, ComPtr<IXAudio2SourceVoice>> pSourceVoice;
//public:
//	Sound();
//	~Sound();
//
//	static void Initialize();
//	static bool Load(const std::string& path,const bool loopFlag,const std::string& name);
//	static Sound* Get(const std::string& name) { return pSound[name].get(); }
//	static void Delete(const std::string& name);
//
//	//void PlayWave(const char* path);
//
//
//	bool LoadSound(const char* path, std::string name, bool loop);
//	void PlayLoadSound(std::string name);
//	void StopLoadSound(std::string name,bool resetFlag);
//	void EraseLoadSound(const std::string& name);
//};
//
