#include "BaseScene.h"

//Raki_DX12B         *BaseScene::dx12b ;
//NY_Object3DManager *BaseScene::objmgr;
//SpriteManager      *BaseScene::spmgr ;

BaseScene::BaseScene(ISceneChanger *changer, SceneChangeDirection* scDirect)  {
    mSceneChanger = changer;
    mSceneChangeDirection = scDirect;
}
