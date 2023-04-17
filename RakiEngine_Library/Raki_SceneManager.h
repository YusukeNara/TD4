#pragma once
#include "ISceneManager.h"

class Raki_SceneManager final
{
private:
	//シーン管理クラス
	static ISceneManager *mgr;

	Raki_SceneManager(){}
	~Raki_SceneManager() {
		delete mgr;
	}

public:


};

