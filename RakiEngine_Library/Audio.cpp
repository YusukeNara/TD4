#include "Audio.h"

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
}

SoundData Audio::LoadSound_wav(const char *filename)
{
    //ファイル入力ストリームのインスタンス
    std::ifstream file;
    //.wavファイルをバイナリモードで開く
    file.open(filename, std::ios_base::binary);
    //オープン失敗を検出
    assert(file.is_open());

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
    if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
        assert(0);
    }
    //チャンク本体の読み込み
    assert(format.chunk.size <= sizeof(format.fmt));
    file.read((char *)&format.fmt, format.chunk.size);

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
    if (strncmp(data.id, "data", 4) != 0) {
        assert(0);
    }
    //dataチャンクのデータ部分読み込み
    char *pBuffer = new char[data.size];
    file.read(pBuffer, data.size);
    //wavを閉じる
    file.close();

    //返却するデータ
    SoundData soundData = {};
    soundData.wfex       = format.fmt;
    soundData.pBuffer    = reinterpret_cast<BYTE*>(pBuffer);
    soundData.bufferSize = data.size;
    HRESULT result;
    //波形フォーマットをもとにソースの作成
    result = xAudio2->CreateSourceVoice(&soundData.source, &soundData.wfex);
    assert(SUCCEEDED(result));

    //再生する波形データの設定
    soundData.buf.pAudioData = soundData.pBuffer;
    soundData.buf.AudioBytes = soundData.bufferSize;
    soundData.buf.Flags = XAUDIO2_END_OF_STREAM;

    return soundData;
}

void Audio::UnloadSound(SoundData *data)
{
    //メモリ解放
    delete[] data->pBuffer;

    data->pBuffer = 0;
    data->bufferSize = 0;
    data->wfex = {};
}

void Audio::SetPlayRoopmode(SoundData &soundData,int roopCount)
{
    if (roopCount < 0 || roopCount>256) { return; }

    soundData.buf.LoopCount = roopCount;
    if (roopCount == 0) {//ループ回数が0の時
        soundData.buf.LoopBegin = 0;
        soundData.buf.LoopLength = 0;
    }
}

void Audio::PlayLoadedSound(const SoundData &soundData, bool isSerialPlay)
{
    HRESULT result;

    XAUDIO2_VOICE_STATE state;
    soundData.source->GetState(&state);
    if (state.BuffersQueued != 0 && !isSerialPlay) { return; }

    //再生中だが連続再生するときは、止めてから再生する
    if (state.BuffersQueued != 0 && isSerialPlay) {
        StopLoadedSound(const_cast<SoundData&>(soundData));
    }

    float playVolume = mastervolume * soundData.volume;
    
    result = soundData.source->SetVolume(playVolume);
    //波形データ再生
    result = soundData.source->SubmitSourceBuffer(&soundData.buf);
    result = soundData.source->Start();
}

void Audio::PauseLoadedSound(const SoundData &soundData)
{
    HRESULT result;

    //波形データ停止
    result = soundData.source->SubmitSourceBuffer(&soundData.buf);
    result = soundData.source->Stop();
}

void Audio::StopLoadedSound(SoundData &soundData)
{
    HRESULT result;

    //波形データ停止
    result = soundData.source->Stop();
    result = soundData.source->FlushSourceBuffers();
    //result = soundData.source->SubmitSourceBuffer(&soundData.buf);
}

void Audio::SetMasterVolume(float volume)
{
    mastervolume = std::clamp(volume, 0.f, 1.0f);
}

void Audio::SetSoundDataVolume(SoundData& data, float volume)
{
    data.volume = std::clamp(volume, 0.f, 1.0f);
}
