#include "SoundData.h"


using namespace MelLib;

std::unordered_map < std::string, std::unique_ptr<SoundData> > SoundData::pSoundDatas;

bool SoundData::Load(const std::string& path,const std::string& name)
{
	pSoundDatas.emplace(name, std::make_unique<SoundData>());
	return pSoundDatas[name]->LoadSound(path, name);
}

void SoundData::Delete(const std::string& name)
{
}

void SoundData::Finalize()
{
	pSoundDatas.clear();
}


bool SoundData::LoadSound(const std::string& path, const std::string& name)
{
#pragma region ファイルオープン
	std::ifstream file;
	file.open(path, std::ios_base::binary);

	if (file.fail())
	{

#ifdef _DEBUG
		OutputDebugStringA(path.c_str());
		OutputDebugStringW(L"の読み込みに失敗しました。pathの入力ミスの可能性があります。\n");
		return false;
#endif // _DEBUG
	}
#pragma endregion

#pragma region 読み込み
	WAVRiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		OutputDebugStringA(path.c_str());
		OutputDebugStringW(L"の読み込みに失敗しました。ファイルからRIFFの文字列を読み込めませんでした。\n");
		return false;
	}

	WAVFormatChunk format;
	file.read((char*)&format, sizeof(format));

	WAVChunk chunk;
	file.read((char*)&chunk, sizeof(chunk));
	bytes = chunk.size;
	pBuffer = new char[bytes];
	file.read(pBuffer, chunk.size);

	file.close();
#pragma endregion

#pragma region 波形フォーマット設定
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;
#pragma endregion

}

SoundData::~SoundData()
{
	delete[] pBuffer;
}
