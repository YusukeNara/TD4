#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "FbxLoader.h"
#include <DirectXMath.h>
#include <cmath>

#include <Raki_Input.h>
#include <NY_Object3DMgr.h>
#include <Sprite.h>

//アニメーションのトータル時間
const int animationCountMax = 600;
const int ScrollCountMax = 60;
//タイトルスプライトの最終位置
const float titleLastPos = -50.0f;

class TitleScene : public BaseScene {

public:
    TitleScene(ISceneChanger *changer, SceneChangeDirection* scd);
    ~TitleScene() override;

    void Initialize() override;    //初期化処理をオーバーライド。
    void Finalize() override ;   //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;          //描画処理をオーバーライド。
    void Draw2D() override;
    void DrawImgui() override;

    void Animation();

    void SceneScroll();

    Sprite testsp;
    Sprite buttonsp;

    std::shared_ptr<Object3d> titleHage;
    std::shared_ptr<Object3d> titlePlayer;
    std::shared_ptr<Object3d> titleAfro;
    std::shared_ptr<Object3d> titleIppon;

    Rv3Ease::Rv3Easing titleEase;

    int animationCount = 0;
    int scrollCount = 0;

    bool isScroll = false;

    XMFLOAT2 titleSpritePos;

    RVector3 titleObjPos;
    RVector3 titlePlayerPos;
    RVector3 titleHagePos;
};

