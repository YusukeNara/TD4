#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"

#include <NY_Object3DMgr.h>
#include <Sprite.h>

class TitleScene : public BaseScene {

public:
    TitleScene(ISceneChanger *changer);
    ~TitleScene() override;

    void Initialize() override;    //初期化処理をオーバーライド。
    void Finalize() override ;   //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;          //描画処理をオーバーライド。
    void Draw2D() override;
    void DrawImgui() override;

    void Animation();

    Sprite testsp;
    std::shared_ptr<Object3d> testobject;

    const int animationCountMax = 1200;
    int animationCount = 0;

    RVector3 testObjPos;
};

