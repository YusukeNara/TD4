#pragma once
#include "Task.h"
#include "ISceneChanger.h"
#include "Raki_DX12B.h"
#include "Raki_WinAPI.h"
#include <wrl.h>

#include "SceneChangeDirection.h"

using namespace Microsoft::WRL;

class SceneChangeDirection;

class BaseScene : public Task {

    //template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

protected:
    
    ISceneChanger *mSceneChanger = nullptr;    //クラス所有元にシーン切り替えを伝えるインターフェイス
    SceneChangeDirection* mSceneChangeDirection = nullptr;

public:
    BaseScene(ISceneChanger* changer, SceneChangeDirection* scDirect);
    BaseScene() {};
    virtual ~BaseScene() {

    }


    virtual void Initialize() override {}    //初期化処理をオーバーライド。
    virtual void Finalize() override {}        //終了処理をオーバーライド。
    virtual void Update() override {}        //更新処理をオーバーライド。
    virtual void Draw() override{}            //描画処理をオーバーライド。
    virtual void Draw2D() override{}
    virtual void DrawImgui() override{}
    virtual void DrawParticle() {}
};

