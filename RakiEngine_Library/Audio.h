#pragma once

#include <xaudio2.h>
#include <fstream>
#include <assert.h>
#include <wrl.h>

#pragma comment(lib,"xaudio2.lib")

using namespace Microsoft::WRL;

//音声データ管理構造体
struct ChunkHeader {
	char id[4];   //チャンクiD
	int32_t size; //チャンクサイズ
};

//RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};

//FMTチャンク
struct FormatChunk {
	ChunkHeader  chunk; //"fmt "
	WAVEFORMATEX fmt;   //波形フォーマット
};

//音声データ
struct SoundData {
	//波形フォーマット
	WAVEFORMATEX wfex;
	//バッファ先頭アドレス
	BYTE *pBuffer;
	//バッファサイズ
	unsigned int bufferSize;
	//オーディオポインタ
	IXAudio2SourceVoice *source;
	//バッファ
	XAUDIO2_BUFFER buf{};
};


class Audio
{
public:

	static ComPtr<IXAudio2>        xAudio2;
	static IXAudio2MasteringVoice *masterVoice;
	static float volume;

public:

	Audio() {
		//xAudioエンジンのインスタンス作成
		HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
		//マスターボイス作成
		result = xAudio2->CreateMasteringVoice(&masterVoice);
		//ボリューム初期化(50%)
		volume = 0.5f;
	}

	//初期化
	static void Init();

	//サウンドデータの読み込み
	static SoundData LoadSound_wav(const char *filename);
	//サウンドデータのアンロード
	static void UnloadSound(SoundData *data);

	//ループの設定(0~254でループ回数を指定。255の場合無限ループ。それ以外は無効)
	static void SetPlayRoopmode(SoundData &soundData,int roopCount);
	//再生
	static void PlayLoadedSound(const SoundData &soundData,bool isSerialPlay = false);
	//一時停止
	static void PauseLoadedSound(const SoundData &soundData);
	//停止
	static void StopLoadedSound(SoundData &soundData);

};

