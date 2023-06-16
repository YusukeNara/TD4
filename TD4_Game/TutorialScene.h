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
};

