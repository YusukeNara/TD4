#include "Audio.h"
#include "RakiUtility.h"
#include "Raki_DX12B.h"

#include <algorithm>

ComPtr<IXAudio2>        Audio::xAudio2;
IXAudio2MasteringVoice *Audio::masterVoice;
float                   Audio::mastervolume;

void Audio::Init()
{
    //xAudioエンジンのインスタンス作成
    HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    //マスターボイス作成
    result = xAudio2->CreateMasteringVoice(&masterVoice);
    //ボリューム初期化(50%)
    mastervolume = 0.5f;

    // MediaFoundation初期化
    result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    result = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
}

SoundData* Audio::LoadSound_wav(const char *filename)
{
    //ファイル入力ストリームのインスタンス
    std::ifstream file;
    //.wavファイルをバイナリモードで開く
    file.open(filename, std::ios_base::binary);
    //オープン失敗を検出
    assert(file.is_open());

    //返却するデータ
    SoundData* soundData = new SoundData();
    HRESULT result;

    //RIFTヘッダー読み込み
    RiffHeader riff;
    file.read((char *)&riff, sizeof(riff));
    //ファイルがRIFTかチェック
    if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
        assert(0);
    }
    //タイプがWAVEかチェック
    if (strncmp(riff.type, "WAVE", 4) != 0) {
        assert(0);
    }

    //フォーマットチャンクの読み込み
    FormatChunk format = {};
    //チャンクヘッダーの確認
    file.read((char *)&format, sizeof(ChunkHeader));
    //JUNKチャンク検出時
    if (strncmp(format.chunk.id, "JUNK", 4) == 0) {
        //読み取りをジャンクチャンクの終わりまで進める
        file.seekg(format.chunk.size, std::ios_base::cur);
        //再読み込み
        file.read((char *)&format, sizeof(ChunkHeader));
    }
    //LISTチャンク検出時
    if (strncmp(format.chunk.id, "LIST", 4) == 0) {
        //読み取りをジャンクチャンクの終わりまで進める
        file.seekg(format.chunk.size, std::ios_base::cur);
        //再読み込み
        file.read((char*)&format, sizeof(ChunkHeader));
    }
    //フォーマットチャンク検出時
    if (strncmp(format.chunk.id, "fmt ", 4) == 0) {
        //チャンク本体の読み込み
        assert(format.chunk.size <= sizeof(format.fmt));
        //先頭4バイトからデータ型を特定
        if (format.chunk.size == int32_t(18)) {
            soundData->byteMode = FMT_BYTE_18;
            file.read((char*)&format.fmt, format.chunk.size);
        }
        else if (format.chunk.size == int32_t(16)) {
            soundData->byteMode = FMT_BYTE_16;
            file.read((char*)&format.pcmwf, format.chunk.size);
        }
        else if (format.chunk.size == int32_t(40)) {
            soundData->byteMode = FMT_BYTE_40;
            file.read((char*)&format.wften, format.chunk.size);
        }

    }

    //Dataチャンクの読み込み
    ChunkHeader data;
    file.read((char *)&data, sizeof(data));
    //JUNKチャンク検出時
    if (strncmp(data.id, "JUNK", 4) == 0) {
        //読み取りをジャンクチャンクの終わりまで進める
        file.seekg(data.size, std::ios_base::cur);
        //再読み込み
        file.read((char *)&data, sizeof(data));
    }
    //LISTチャンク検出時
    if (strncmp(data.id, "LIST", 4) == 0) {
        //読み取りをジャンクチャンクの終わりまで進める
        file.seekg(data.size, std::ios_base::cur);
        //再読み込み
        file.read((char*)&data, sizeof(data));
    }
    if (strncmp(data.id, "data", 4) != 0) {
        assert(0);
    }
    //dataチャンクのデータ部分読み込み
    char *pBuffer = new char[data.size];
    file.read(pBuffer, data.size);
    //wavを閉じる
    file.close();

    switch (soundData->byteMode)
    {
    case FMT_BYTE_16:
        //WAVEFORMATEXに合わせる
        soundData->wfex.wFormatTag = format.pcmwf.wf.wFormatTag;
        soundData->wfex.nChannels = format.pcmwf.wf.nChannels;
        soundData->wfex.nSamplesPerSec = format.pcmwf.wf.nSamplesPerSec;
        soundData->wfex.nBlockAlign = format.pcmwf.wf.nBlockAlign;
        soundData->wfex.nAvgBytesPerSec = format.pcmwf.wf.nAvgBytesPerSec;
        soundData->wfex.wBitsPerSample = format.pcmwf.wBitsPerSample;
        //波形フォーマットをもとにソースの作成
        result = xAudio2->CreateSourceVoice(&soundData->source, &soundData->wfex);
        ExportHRESULTmessage(result);
        break;
    case FMT_BYTE_18:
        soundData->wfex = format.fmt;
        //波形フォーマットをもとにソースの作成
        result = xAudio2->CreateSourceVoice(&soundData->source, &soundData->wfex);
        break;
    case FMT_BYTE_40:
        //WAVEFORMATEXに合わせる
        soundData->wfex = format.wften.Format;
        soundData->wfex.wFormatTag = WORD(format.wften.SubFormat.Data1);
        //波形フォーマットをもとにソースの作成
        result = xAudio2->CreateSourceVoice(&soundData->source, &soundData->wfex);
        break;
    default:
        break;
    }


    soundData->pBuffer    = reinterpret_cast<BYTE*>(pBuffer);
    soundData->bufferSize = data.size;


    assert(SUCCEEDED(result));

    //再生する波形データの設定
    soundData->buf.pAudioData = soundData->pBuffer;
    soundData->buf.AudioBytes = soundData->bufferSize;
    soundData->buf.Flags = XAUDIO2_END_OF_STREAM;

    return soundData;
}

SoundData Audio::LoadSound_mp3(const char* filename)
{
    HRESULT result;

    IMFSourceReader* pMFSourceReader{ nullptr };

    std::wstring path = rutility::charTowstring(filename);

    result = MFCreateSourceReaderFromURL(path.c_str(), NULL, &pMFSourceReader);
    ExportHRESULTmessage(result);
    

    IMFMediaType* pMFMediaType{ nullptr };
    MFCreateMediaType(&pMFMediaType);
    pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    pMFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType);

    pMFMediaType->Release();
    pMFMediaType = nullptr;
    pMFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pMFMediaType);

    SoundData sounddata;
    //MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &sounddata.wfex, nullptr);

    std::vector<BYTE> mediaData;
    while (true)
    {
        IMFSample* pMFSample{ nullptr };
        DWORD dwStreamFlags{ 0 };
        pMFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

        if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
        {
            break;
        }

        IMFMediaBuffer* pMFMediaBuffer{ nullptr };
        pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

        BYTE* pBuffer{ nullptr };
        DWORD cbCurrentLength{ 0 };
        pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

        mediaData.resize(mediaData.size() + cbCurrentLength);
        memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

        pMFMediaBuffer->Unlock();

        pMFMediaBuffer->Release();
        pMFSample->Release();
    }

    //ソースに格納
    //xAudio2->CreateSourceVoice(&sounddata.source, sounddata.wfex);
    assert(SUCCEEDED(result));
    sounddata.pBuffer = reinterpret_cast<BYTE*>(mediaData.data());
    //バッファに格納
    sounddata.buf.pAudioData = mediaData.data();
    sounddata.buf.Flags = XAUDIO2_END_OF_STREAM;
    sounddata.buf.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(mediaData.size());

    return sounddata;
}

void Audio::UnloadSound(SoundData *data)
{
    //メモリ解放
    delete[] data->pBuffer;
    data->source->FlushSourceBuffers();

    data->pBuffer = 0;
    data->bufferSize = 0;
    data->wfex = {};
}

void Audio::SetPlayRoopmode(SoundData* soundData,int roopCount)
{
    if (roopCount < 0 || roopCount>256) { return; }

    soundData->buf.LoopCount = roopCount;
    if (roopCount == 0) {//ループ回数が0の時
        soundData->buf.LoopBegin = 0;
        soundData->buf.LoopLength = 0;
    }
}

void Audio::PlayLoadedSound(SoundData* soundData, bool isSerialPlay)
{
    HRESULT result;
    XAUDIO2_VOICE_STATE state;

    if (soundData->isPause) {
        result = soundData->source->Start();
        soundData->isPause = false;
    }
    else {
        soundData->source->GetState(&state);
        if (state.BuffersQueued != 0 && !isSerialPlay) { return; }

        //再生中だが連続再生するときは、止めてから再生する
        if (state.BuffersQueued != 0 && isSerialPlay) {
            StopLoadedSound(const_cast<SoundData*>(soundData));
        }

        float playVolume = float(std::clamp(double(mastervolume * soundData->volume), 0.0, 1.0));

        result = soundData->source->SetVolume(playVolume);
        //波形データ再生
        result = soundData->source->SubmitSourceBuffer(&soundData->buf);
        result = soundData->source->Start();
    }
}

void Audio::PauseLoadedSound(SoundData *soundData)
{
    //一時停止フラグ
    soundData->isPause = true;

    HRESULT result;

    //波形データ停止
    result = soundData->source->Stop();
}

void Audio::StopLoadedSound(SoundData *soundData)
{
    soundData->isPause = false;

    HRESULT result;

    //波形データ停止
    result = soundData->source->Stop();
    result = soundData->source->FlushSourceBuffers();
}

void Audio::SetMasterVolume(float volume)
{
    mastervolume = std::clamp(volume, 0.f, 1.0f);
}

void Audio::SetSoundDataVolume(SoundData& data, float volume)
{
    data.volume = std::clamp(volume, 0.f, 1.0f);
}

SoundData::~SoundData()
{
    delete[] pBuffer;
}
