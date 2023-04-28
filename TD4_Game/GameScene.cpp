#include "GameScene.h"

GameScene::GameScene(ISceneChanger *changer) : BaseScene(changer) {


}

//‰Šú‰»
void GameScene::Initialize() {

}

void GameScene::Finalize()
{
}

//XV
void GameScene::Update() {



}

//•`‰æ
void GameScene::Draw() {

	Raki_DX12B::Get()->StartDraw();

	Raki_DX12B::Get()->EndDraw();
}

void GameScene::Draw2D()
{
}
