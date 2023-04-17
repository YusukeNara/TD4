#pragma once

#include "SceneBase.h"

//FactoryMethod採用シーン管理部

//シーンを作成する抽象クラス（シーン作成工場の概念）
class AbstructSceneFactory 
{
public:
	/// <summary>
	/// シーン生成関数（純粋仮想関数）
	/// </summary>
	/// <returns>生成したシーンのポインタ</returns>
	virtual SceneBase *CreateScene() = 0;


};

