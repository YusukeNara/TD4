#include "AbstructSceneFactory.h"

#pragma once

//FactoryMethod採用シーン管理部
//シーン作成工場の具体部分
class SceneFactory : public AbstructSceneFactory
{
public:
	/// <summary>
	/// シーン生成関数（純粋仮想関数）
	/// </summary>
	/// <returns>生成したシーンのポインタ</returns>
	SceneBase *CreateScene() override;


};

