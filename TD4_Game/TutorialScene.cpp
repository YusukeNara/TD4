#include "TutorialScene.h"

TutorialScene::TutorialScene(ISceneChanger *changer) : BaseScene(changer)
{
	groundTex = TexManager::LoadTexture("Resources/asp3.png");

	groundObject = std::make_shared<Object3d>();
	groundObject.reset(NY_Object3DManager::Get()->CreateModel_Tile(500.f, 500.f, 10.f, 10.f, groundTex));

	groundObject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));


	player = std::make_unique<ProtoPlayer>();
	headMan = std::make_unique<HeadManager>(player.get());
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Initialize()
{
	player->Init();

	RVector3 easeOffset(0, 0, 100.f);
	headMan->TutorialInit();
}

void TutorialScene::Finalize()
{
}

void TutorialScene::Update()
{

	player->Update();

	headMan->TutorialUpdate();
	if (Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_R)) {
		mSceneChanger->ChangeScene(eScene_Game);
	}
}

void TutorialScene::Draw()
{
	headMan->Draw();
}

void TutorialScene::Draw2D()
{
	player->DrawUI();
	headMan->DrawUI();
}

void TutorialScene::DrawImgui()
{
}
