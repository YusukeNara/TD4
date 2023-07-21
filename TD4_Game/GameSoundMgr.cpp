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
			cutSe[i].reset(Audio::LoadSound_wav(cutfilepass.c_str()));
		}
		slapSe[i].reset(Audio::LoadSound_wav(slapfilepass.c_str()));
	}
	pullSe.reset(Audio::LoadSound_wav("Resources/sounds/se/pull.wav"));
	buttonSe.reset(Audio::LoadSound_wav("Resources/sounds/se/ok.wav"));
	cancelSe.reset(Audio::LoadSound_wav("Resources/sounds/se/cancel.wav"));

	titleBgm.reset(Audio::LoadSound_wav("Resources/sounds/bgm/titlebgm.wav"));
	gameBgm.reset(Audio::LoadSound_wav("Resources/sounds/bgm/gamebgm.wav"));
	resultBgm.reset(Audio::LoadSound_wav("Resources/sounds/bgm/resultbgm.wav"));

	Audio::SetPlayRoopmode(titleBgm.get(), 255);
	Audio::SetPlayRoopmode(gameBgm.get(), 255);
}

void GameSoundMgr::PlayTitleBGM()
{
	Audio::PlayLoadedSound(titleBgm.get());
}

void GameSoundMgr::StopTitleBGM()
{
	Audio::StopLoadedSound(titleBgm.get());
}

void GameSoundMgr::PlayGameBGM()
{
	Audio::PlayLoadedSound(gameBgm.get());
}

void GameSoundMgr::StopGameBGM()
{
	Audio::StopLoadedSound(gameBgm.get());
}

void GameSoundMgr::PlayResultBGM()
{
	Audio::PlayLoadedSound(resultBgm.get());
}

void GameSoundMgr::StopResultBGM()
{
	Audio::StopLoadedSound(resultBgm.get());
}

void GameSoundMgr::PlayCutSE()
{
	int playnum = NY_random::intrand_sl(1, 0);
	Audio::PlayLoadedSound(cutSe[playnum].get(), true);
}

void GameSoundMgr::PlaySlapSE()
{
	int playnum = NY_random::intrand_sl(2, 0);
	Audio::PlayLoadedSound(slapSe[playnum].get(), true);
}

void GameSoundMgr::PlayPullSE()
{
	Audio::PlayLoadedSound(pullSe.get(), true);
}

void GameSoundMgr::PlayButtonSE()
{
	Audio::PlayLoadedSound(buttonSe.get(), true);
}

void GameSoundMgr::PlayCancelSE()
{
	Audio::PlayLoadedSound(cancelSe.get(), true);
}
