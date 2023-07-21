#pragma once

#include <xaudio2.h>
#include <fstream>
#include <assert.h>
#include <wrl.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

// mediafoundation
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

// xaudio
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
	ChunkHeader				chunk; //"fmt "
	WAVEFORMAT				wf;
	PCMWAVEFORMAT			pcmwf;
	WAVEFORMATEX			fmt;   //波形フォーマット
	WAVEFORMATEXTENSIBLE	wften;
};

enum FMT_BYTE {
	FMT_BYTE_16,
	FMT_BYTE_18,
	FMT_BYTE_40,
};

//音声データ
struct SoundData {
	//波形フォーマット（大きさに応じてデータ型が異なる、なんでだよ）
	WAVEFORMAT				wf;
	PCMWAVEFORMAT			pcmwf;
	WAVEFORMATEX			wfex;
	WAVEFORMATEXTENSIBLE	wften;

	FMT_BYTE byteMode;

	//バッファ先頭アドレス
	BYTE *pBuffer = nullptr;
	//バッファサイズ
	unsigned int bufferSize;
	//オーディオポインタ
	IXAudio2SourceVoice *source = nullptr;
	//バッファ
	XAUDIO2_BUFFER buf{};
	//音量
	float volume = 1.0f;
	//一時停止フラグ
	bool isPause = false;

	SoundData(){}
	~SoundData();
};


class Audio
{
private:
	// XAudio
	static ComPtr<IXAudio2>        xAudio2;
	static IXAudio2MasteringVoice *masterVoice;
	static float mastervolume;

	static SoundData LoadSound_mp3(const char* filename);

public:

	Audio() {
		//xAudioエンジンのインスタンス作成
		HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
		//マスターボイス作成
		result = xAudio2->CreateMasteringVoice(&masterVoice);
		//ボリューム初期化(50%)
		mastervolume = 0.5f;
	}

	//初期化
	static void Init();

	//サウンドデータの読み込み
	static SoundData *LoadSound_wav(const char* filename);


	//サウンドデータのアンロード
	static void UnloadSound(SoundData *data);
	

	//ループの設定(0~254でループ回数を指定。255の場合無限ループ。それ以外は無効)
	static void SetPlayRoopmode(SoundData *soundData,int roopCount);
	//再生
	static void PlayLoadedSound(SoundData *soundData,bool isSerialPlay = false);
	//一時停止
	static void PauseLoadedSound(SoundData *soundData);
	//停止
	static void StopLoadedSound(SoundData *soundData);

	//マスター音量変更
	static void SetMasterVolume(float volume);
	//音データ音量変更
	static void SetSoundDataVolume(SoundData& data, float volume);
};

