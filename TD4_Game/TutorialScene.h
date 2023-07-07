#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "FbxLoader.h"
#include <DirectXMath.h>
#include <cmath>

#include <Raki_Input.h>
#include <NY_Object3DMgr.h>
#include <Sprite.h>

#include "Head.h"
#include "ProtoPlayer.h"
#include"HeadManager.h"

class TutorialScene : public BaseScene
{
public:
	TutorialScene(ISceneChanger *changer);
	~TutorialScene() override;
    void Initialize() override;    //初期化処理をオーバーライド。
    void Finalize() override;   //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;          //描画処理をオーバーライド。
    void Draw2D() override;
    void DrawImgui() override;

private:
    std::unique_ptr<HeadManager> headMan;

    std::unique_ptr<ProtoPlayer> player;

    //その他
    std::shared_ptr<Object3d> groundObject;

    UINT groundTex;

    int tutorialNum;

    Sprite tutorialTitle;
    Sprite storeManSprite;
    Sprite hagesiSprite;
    Sprite tutorialSprite1;
    Sprite tutorialSprite2;
    Sprite tutorialSprite3;
    Sprite tutorialSprite4;
    Sprite tutorialSprite5;

    Sprite tutorialSprite6;
    Sprite tutorialSprite7;
    Sprite tutorialSprite8;

    Sprite tutorialSprite9;
    Sprite tutorialSprite10;

    Sprite tutorialSprite11;
    Sprite tutorialSprite12;

    Sprite tutorialSprite13;
    Sprite tutorialSprite14;
    Sprite tutorialSprite15;
    Sprite tutorialSprite16;
    Sprite tutorialSprite17;

    Sprite tutorialSprite18;

    Sprite tutorialSprite19;
    Sprite tutorialSprite20;

    Sprite hirightSprite1;
    Sprite hirightSprite2;
    Sprite hirightSprite3;

    Sprite tutorialSprite21;
    Sprite tutorialSprite22;

    Sprite tutorialSprite23;
    Sprite tutorialSprite24;
    Sprite tutorialSprite25;
};

