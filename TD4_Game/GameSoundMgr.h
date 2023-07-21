#pragma once
#include <Audio.h>

#include <array>

class GameSoundMgr
{
private:
	//各音データ
	SoundData titleBgm = {};
	SoundData gameBgm = {};
	SoundData resultBgm = {};

	std::array<SoundData, 3> slapSe = {};
	std::array<SoundData, 2> cutSe = {};
	SoundData pullSe = {};
	SoundData buttonSe = {};
	SoundData cancelSe = {};


	GameSoundMgr() = default;
	~GameSoundMgr();

public:
	static GameSoundMgr *get()
	{
		static GameSoundMgr* ins;
		return ins;
	}

	void Init();

	//BGM
	void PlayTitleBGM();
	void StopTitleBGM();
	void PlayGameBGM();
	void StopGameBGM();
	void PlayResultBGM();
	void StopResultBGM();

	//SE
	void PlayCutSE();
	void PlaySlapSE();
	void PlayPullSE();
	void PlayButtonSE();
	void PlayCancelSE();


};

