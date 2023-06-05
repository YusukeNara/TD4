#pragma once
#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "FbxLoader.h"
#include <DirectXMath.h>
#include <cmath>

#include <Raki_Input.h>
#include <NY_Object3DMgr.h>
#include <Sprite.h>

class ResultScene : public BaseScene
{
public:
	ResultScene(ISceneChanger* changer);
	~ResultScene() override;

    void Initialize() override;    //初期化処理をオーバーライド。
    void Finalize() override;   //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;          //描画処理をオーバーライド。
    void Draw2D() override;
    void DrawImgui() override;

    void Animation();

    Sprite spriteReview;
    Sprite spriteScore;

    std::shared_ptr<Object3d> resultPlayer;

    const int animationCountMax = 600;
    int animationCount = 0;
    bool starIsDraw;
    bool scoreTexIsDraw;

    //総合的なスコア
    float totalScore;
    //総合的なレビュー
    float totalReview;
    //さばいた数
    int handleNum;

    XMFLOAT2 starPos;

    //捌いた数の位置
    const XMFLOAT2 HANDLE_POS = { 512,0 };
    const XMFLOAT2 TOTALREVIEW_POS = { 512,64 };
    const float SCORE_POS_X = 765.0f;
    const float SCORE_POS_Y = 360.0f;
};

