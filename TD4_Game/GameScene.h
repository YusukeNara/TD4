#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "NY_Object3DMgr.h"
#include "Raki_Input.h"

class GameScene : public BaseScene {
public:
    GameScene(ISceneChanger *changer);
    void Initialize() override;    //初期化処理をオーバーライド。
    void Finalize() override;        //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;            //描画処理をオーバーライド。
    void Draw2D() override;

    ////モデル
    //Model3D boxModel;
    ////オブジェクト
    //Object3d boxObject;

};

