#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "NY_Object3DMgr.h"
#include "Raki_Input.h"
#include "GameManager.h"

#include <NY_Object3DMgr.h>
#include <Sprite.h>

class GameScene : public BaseScene {
public:
    GameScene(ISceneChanger *changer);
    ~GameScene() override;
    void Initialize() override;    //初期化処理をオーバーライド。
    void Finalize() override;        //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;            //描画処理をオーバーライド。
    void Draw2D() override;
    void DrawImgui() override;

    Sprite testsp;
    //std::shared_ptr<Object3d> testobject;

    GameManager gmgr;
};

