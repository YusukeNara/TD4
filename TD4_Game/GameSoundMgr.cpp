#include "GameSoundMgr.h"

#include <string>
#include <NY_random.h>

GameSoundMgr::~GameSoundMgr()
{
}

void GameSoundMgr::Init()
{
	std::string pass = "Resources/sounds/";
	std::string sefolder = "se/";


	std::string ext = ".wav";
	for (int i = 0; i < 3; i++) {
		std::string slapfilename = "slap";
		std::string cutfilename = "cut";
		std::string num = std::to_string(i + 1);

		std::string cutfilepass = pass + sefolder + cutfilename + num + ext;
		std::string slapfilepass = pass + sefolder + slapfilename + num + ext;

		if (i < 2) {
			cutSe[i] = Audio::LoadSound_wav(cutfilepass.c_str());
		}
		slapSe[i] = Audio::LoadSound_wav(slapfilepass.c_str());
	}
	pullSe = Audio::LoadSound_wav("Resources/sounds/se/pull.wav");
	buttonSe = Audio::LoadSound_wav("Resources/sounds/se/ok.wav");
	cancelSe = Audio::LoadSound_wav("Resources/sounds/se/cancel.wav");

	titleBgm = Audio::LoadSound_wav("Resources/sounds/bgm/titlebgm.wav");
	gameBgm = Audio::LoadSound_wav("Resources/sounds/bgm/gamebgm.wav");
	resultBgm = Audio::LoadSound_wav("Resources/sounds/bgm/resultbgm.wav");

	Audio::SetPlayRoopmode(titleBgm, 255);
	Audio::SetPlayRoopmode(gameBgm, 255);
}

void GameSoundMgr::PlayTitleBGM()
{
	Audio::PlayLoadedSound(titleBgm);
}

void GameSoundMgr::StopTitleBGM()
{
	Audio::StopLoadedSound(titleBgm);
}

void GameSoundMgr::PlayGameBGM()
{
	Audio::PlayLoadedSound(gameBgm);
}

void GameSoundMgr::StopGameBGM()
{
	Audio::StopLoadedSound(gameBgm);
}

void GameSoundMgr::PlayResultBGM()
{
	Audio::PlayLoadedSound(resultBgm);
}

void GameSoundMgr::StopResultBGM()
{
	Audio::StopLoadedSound(resultBgm);
}

void GameSoundMgr::PlayCutSE()
{
	int playnum = NY_random::intrand_sl(1, 0);
	Audio::PlayLoadedSound(cutSe[playnum], true);
}

void GameSoundMgr::PlaySlapSE()
{
	int playnum = NY_random::intrand_sl(2, 0);
	Audio::PlayLoadedSound(slapSe[playnum], true);
}

void GameSoundMgr::PlayPullSE()
{
	Audio::PlayLoadedSound(pullSe, true);
}

void GameSoundMgr::PlayButtonSE()
{
	Audio::PlayLoadedSound(buttonSe, true);
}

void GameSoundMgr::PlayCancelSE()
{
	Audio::PlayLoadedSound(cancelSe, true);
}
