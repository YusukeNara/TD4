#pragma once
#include "BaseScene.h"

#include <NY_Object3DMgr.h>
#include <Sprite.h>
#include <Raki_Input.h>
#include <ParticleManager.h>
#include <CameraCalc.h>
#include <Audio.h>

//エンジン側検証用シーン
//このクラスもいじらないこと
class EngineDebugScene : public BaseScene
{
public:
    EngineDebugScene(ISceneChanger* changer, SceneChangeDirection* scDirect);
    ~EngineDebugScene() override;
    void Initialize() override;    //初期化処理をオーバーライド。
    void Finalize() override;        //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;            //描画処理をオーバーライド。
    void Draw2D() override;
    void DrawImgui() override;
    void DrawParticle() override;

    Sprite testsp1;
    Sprite testsp2;
    std::shared_ptr<Object3d> testobject;

    std::shared_ptr<Object3d> testFBX_YesBone;
    std::shared_ptr<Object3d> testFBX_NoBone;

    std::shared_ptr<Object3d> testobj;

    Rv3Ease::Rv3Easing testEase;

    int testmode = 0;
    int testanimnum = 0;

    //デバッグ用変数
    float rotX = 0.f, rotY = 0.f, rotZ = 0.f;

    float camrot = 0.f;

    CameraCalc testcam;

    RQuaternion q1, q2;

    RVector3 lightdir;

    //音
    std::unique_ptr<SoundData> testSE;
    std::unique_ptr<SoundData> testBGM;
    //音テスト用



    Sprite testNum;
    int dval = 0;

    //パーティクル
    std::unique_ptr<ParticleManager> testp;
    UINT particleTex;
    ParticleGrainState pgstate;

    //スプライン
    Rv3Ease::Rv3Spline testspline;
    std::array<RVector3, 6> controlPoint;

    Rv3Ease::Rv3Bezier3 testbezier;

    int activeAnimation = 0;
    int differentAnimation = 0;
    bool isRoop = false;

};