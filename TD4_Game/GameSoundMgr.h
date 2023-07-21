#pragma once
#include <Audio.h>

#include <array>

class GameSoundMgr
{
private:
	//各音データ
	std::unique_ptr<SoundData> titleBgm;
	std::unique_ptr<SoundData> gameBgm;
	std::unique_ptr<SoundData> resultBgm;

	std::array<std::unique_ptr<SoundData>, 3> slapSe = {};
	std::array<std::unique_ptr<SoundData>, 2> cutSe = {};
	std::unique_ptr<SoundData> pullSe;
	std::unique_ptr<SoundData> buttonSe;
	std::unique_ptr<SoundData> cancelSe;


	GameSoundMgr() = default;
	~GameSoundMgr();

public:
	static GameSoundMgr *get()
	{
		static GameSoundMgr ins;
		return &ins;
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

