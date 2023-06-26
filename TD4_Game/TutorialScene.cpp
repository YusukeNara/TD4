#include "TutorialScene.h"

TutorialScene::TutorialScene(ISceneChanger *changer) : BaseScene(changer)
{
	groundTex = TexManager::LoadTexture("Resources/asp3.png");

	groundObject = std::make_shared<Object3d>();
	groundObject.reset(NY_Object3DManager::Get()->CreateModel_Tile(500.f, 500.f, 10.f, 10.f, groundTex));

	groundObject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));


	player = std::make_unique<ProtoPlayer>();
	headMan = std::make_unique<HeadManager>(player.get());

	tutorialTitle.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString1.png"));
	storeManSprite.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString2.png"));
	tutorialSprite1.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString3.png"));
	hagesiSprite.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString4.png"));
	tutorialSprite2.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString5.png"));
	tutorialSprite3.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString6.png"));
	tutorialSprite4.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString7.png"));
	tutorialSprite5.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString8.png"));

	tutorialSprite6.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString9.png"));
	tutorialSprite7.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString10.png"));
	tutorialSprite8.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString11.png"));

	tutorialSprite9.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString12.png"));
	tutorialSprite10.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString13.png"));

	tutorialSprite11.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString14.png"));
	tutorialSprite12.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString15.png"));

	tutorialSprite13.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString16.png"));
	tutorialSprite14.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString17.png"));
	tutorialSprite15.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString17.png"));
	tutorialSprite16.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString17.png"));
	tutorialSprite17.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString18.png"));

	tutorialSprite18.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString19.png"));

	tutorialSprite19.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString20.png"));
	tutorialSprite20.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString21.png"));

	hirightSprite1.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString22.png"));

}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Initialize()
{
	player->Init();

	RVector3 easeOffset(0, 0, 100.f);
	headMan->TutorialInit();

	tutorialNum = 0;
}

void TutorialScene::Finalize()
{
}

void TutorialScene::Update()
{
	if (Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_R)) {
		mSceneChanger->ChangeScene(eScene_Game);
	}
	if (tutorialNum >= 6)
	{
		player->Update();

		headMan->TutorialUpdate();
	}
}

void TutorialScene::Draw()
{
	if (tutorialNum >= 6)
	{
		headMan->Draw();
	}
}

void TutorialScene::Draw2D()
{
	if (tutorialNum > 6)
	{
		player->DrawUI();
		headMan->DrawUI();
	}
	switch (tutorialNum)
	{
	case 0:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		tutorialTitle.DrawSprite(1280 / 2 - 448 / 2,720 / 2 - 64 / 2);
		tutorialTitle.Draw();
		break;
	case 1:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		storeManSprite.DrawSprite(0, 450);
		storeManSprite.Draw();

		tutorialSprite1.DrawSprite(32, 520);
		tutorialSprite1.Draw();

		break;
	case 2:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite2.DrawSprite(32, 520);
		tutorialSprite2.Draw();
		
		tutorialSprite3.DrawSprite(32, 584);
		tutorialSprite3.Draw();

		break;
	case 3:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		storeManSprite.DrawSprite(0, 450);
		storeManSprite.Draw();

		tutorialSprite4.DrawSprite(32, 520);
		tutorialSprite4.Draw();

		tutorialSprite5.DrawSprite(32, 584);
		tutorialSprite5.Draw();
		break;
	case 4:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite6.DrawSprite(32, 520);
		tutorialSprite6.Draw();

		tutorialSprite7.DrawSprite(32, 584);
		tutorialSprite7.Draw();

		tutorialSprite8.DrawSprite(32, 648);
		tutorialSprite8.Draw();

		break;
	case 5:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite9.DrawSprite(32, 520);
		tutorialSprite9.Draw();

		tutorialSprite10.DrawSprite(32, 584);
		tutorialSprite10.Draw();
		break;
	case 6:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite11.DrawSprite(32, 520);
		tutorialSprite11.Draw();

		tutorialSprite12.DrawSprite(32, 584);
		tutorialSprite12.Draw();
		break;
	case 7:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite13.DrawSprite(32, 520);
		tutorialSprite13.Draw();

		break;
	case 8:
		if (Input::Get()->isKeyTrigger(DIK_W) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_Y))
		{
			tutorialNum++;
		}
		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(145, 570, { 0,0,0,0.8f });
		hirightSprite1.Draw();
		break;
	case 9:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite13.DrawSprite(32, 520);
		tutorialSprite13.Draw();

		tutorialSprite14.DrawSprite(224, 520);
		tutorialSprite14.Draw();
		break;
	case 10:
		if (Input::Get()->isKeyTrigger(DIK_W) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_Y))
		{
			tutorialNum++;
		}
		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(145, 545, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		break;

	case 11:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite13.DrawSprite(32, 520);
		tutorialSprite13.Draw();

		tutorialSprite14.DrawSprite(224, 520);
		tutorialSprite14.Draw();

		tutorialSprite15.DrawSprite(416, 520);
		tutorialSprite15.Draw();
		break;
	case 12:
		if (Input::Get()->isKeyTrigger(DIK_W) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_Y))
		{
			tutorialNum++;
		}
		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(145, 545, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		break;
	case 13:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite13.DrawSprite(32, 520);
		tutorialSprite13.Draw();

		tutorialSprite14.DrawSprite(224, 520);
		tutorialSprite14.Draw();

		tutorialSprite15.DrawSprite(416, 520);
		tutorialSprite15.Draw();

		tutorialSprite16.DrawSprite(608, 520);
		tutorialSprite16.Draw();

		break;

	case 14:
		if (Input::Get()->isKeyTrigger(DIK_A) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_X))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(25, 570, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		break;
	case 15:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		//headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite13.DrawSprite(32, 520);
		tutorialSprite13.Draw();

		tutorialSprite14.DrawSprite(224, 520);
		tutorialSprite14.Draw();

		tutorialSprite15.DrawSprite(416, 520);
		tutorialSprite15.Draw();

		tutorialSprite16.DrawSprite(608, 520);
		tutorialSprite16.Draw();

		tutorialSprite17.DrawSprite(32, 584);
		tutorialSprite17.Draw();
		break;


	case 16:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite13.DrawSprite(32, 520);
		tutorialSprite13.Draw();
		break;

	case 17:
		if (Input::Get()->isKeyTrigger(DIK_D) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_B))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(265, 570, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		break;
	case 18:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite13.DrawSprite(32, 520);
		tutorialSprite13.Draw();

		tutorialSprite18.DrawSprite(224, 520);
		tutorialSprite18.Draw();

		break;

	case 19:
		if (Input::Get()->isKeyTrigger(DIK_A) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_X))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(25, 570, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		break;
	case 20:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		//headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite13.DrawSprite(32, 520);
		tutorialSprite13.Draw();

		tutorialSprite18.DrawSprite(224, 520);
		tutorialSprite18.Draw();

		tutorialSprite17.DrawSprite(32, 584);
		tutorialSprite17.Draw();
		break;
	case 21:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite13.DrawSprite(32, 520);
		tutorialSprite13.Draw();
		break;
	case 22:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite13.DrawSprite(32, 520);
		tutorialSprite13.Draw();

		tutorialSprite19.DrawSprite(224, 520);
		tutorialSprite19.Draw();
		break;
	case 23:
		if (Input::Get()->isKeyTrigger(DIK_A) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_X))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(25, 545, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		break;
	case 24:
		if (Input::Get()->isKeyTrigger(DIK_SPACE) || Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			tutorialNum++;
		}

		//headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		tutorialSprite13.DrawSprite(32, 520);
		tutorialSprite13.Draw();

		tutorialSprite19.DrawSprite(224, 520);
		tutorialSprite19.Draw();

		tutorialSprite20.DrawSprite(32, 584);
		tutorialSprite20.Draw();
		break;

	default:
		break;
	}
}

void TutorialScene::DrawImgui()
{
}
