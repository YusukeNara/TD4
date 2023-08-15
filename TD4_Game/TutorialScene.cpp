#include "TutorialScene.h"

#include "GameSoundMgr.h"

TutorialScene::TutorialScene(ISceneChanger *changer, SceneChangeDirection* scd) : BaseScene(changer,scd)
{
	groundTex = TexManager::LoadTexture("Resources/asp3.png");

	groundObject = std::make_shared<Object3d>();
	groundObject.reset(NY_Object3DManager::Get()->CreateModel_Tile(500.f, 500.f, 10.f, 10.f, groundTex));

	groundObject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));


	player = std::make_unique<ProtoPlayer>();
	headMan = std::make_unique<HeadManager>(player.get());

	tutorialTitle.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString1.png"));
	storeManSprite.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString2.png"));
	tutorialSprite1.CreateAndSetDivisionUVOffsets(10,10,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString3.png"));
	hagesiSprite.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString4.png"));
	tutorialSprite2.CreateAndSetDivisionUVOffsets(7,7,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString5.png"));
	tutorialSprite3.CreateAndSetDivisionUVOffsets(17,17,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString6.png"));
	tutorialSprite4.CreateAndSetDivisionUVOffsets(13,13,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString7.png"));
	tutorialSprite5.CreateAndSetDivisionUVOffsets(12,12,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString8.png"));

	tutorialSprite6.CreateAndSetDivisionUVOffsets(8,8,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString9.png"));
	tutorialSprite7.CreateAndSetDivisionUVOffsets(18,18,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString10.png"));
	tutorialSprite8.CreateAndSetDivisionUVOffsets(13,13,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString11.png"));

	tutorialSprite9.CreateAndSetDivisionUVOffsets(8,8,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString12.png"));
	tutorialSprite10.CreateAndSetDivisionUVOffsets(16,16,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString13.png"));

	tutorialSprite11.CreateAndSetDivisionUVOffsets(11,11,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString14.png"));
	tutorialSprite12.CreateAndSetDivisionUVOffsets(18,18,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString15.png"));

	tutorialSprite13.CreateAndSetDivisionUVOffsets(3,3,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString16.png"));
	tutorialSprite14.CreateAndSetDivisionUVOffsets(3,3,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString17.png"));
	tutorialSprite15.CreateAndSetDivisionUVOffsets(3, 3, 1, 64, 64, TexManager::LoadTexture("Resources/tutorialScene/titleString17.png"));
	tutorialSprite16.CreateAndSetDivisionUVOffsets(3, 3, 1, 64, 64, TexManager::LoadTexture("Resources/tutorialScene/titleString17.png"));
	tutorialSprite17.CreateAndSetDivisionUVOffsets(14,14,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString18.png"));

	tutorialSprite18.CreateAndSetDivisionUVOffsets(16,16,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString19.png"));

	tutorialSprite19.CreateAndSetDivisionUVOffsets(8,8,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString20.png"));
	tutorialSprite20.CreateAndSetDivisionUVOffsets(6,6,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString21.png"));

	hirightSprite1.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString22.png"));

	tutorialSprite21.CreateAndSetDivisionUVOffsets(14,14,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString23.png"));
	tutorialSprite22.CreateAndSetDivisionUVOffsets(17,17,1,64,64,TexManager::LoadTexture("Resources/tutorialScene/titleString24.png"));

	tutorialSprite23.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString25.png"));
	tutorialSprite24.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString26.png"));
	tutorialSprite25.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString27.png"));

	tutorialSprite26.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString28.png"));

	tutorialSprite27.Create(TexManager::LoadTexture("Resources/tutorialScene/titleString29.png"));
}

TutorialScene::~TutorialScene()
{
	GameSoundMgr::get()->StopTutorialBGM();
}

void TutorialScene::Initialize()
{
	player->Init();

	RVector3 easeOffset(0, 0, 100.f);
	headMan->TutorialInit();

	tutorialNum = 0;
	aTimer = 0;

	textSpeed = 0;
	textNum = 0;
}

void TutorialScene::Finalize()
{
	GameSoundMgr::get()->StopTutorialBGM();
}

void TutorialScene::Update()
{
	if (Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_R)) {
		mSceneChanger->ChangeScene(eScene_Game);
	}

	aTimer++;
	if (aTimer > 80)
	{
		aTimer = 0;
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
	player->Draw();
}

void TutorialScene::Draw2D()
{
	tutorialSprite26.DrawExtendSprite(900, 0, 1280, 100);
	tutorialSprite26.Draw();

	if (tutorialNum > 6)
	{
		player->DrawUI();
		headMan->DrawUI();
	}
	switch (tutorialNum)
	{
	case 0:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
		}

		tutorialTitle.DrawSprite(1280 / 2 - 448 / 2,720 / 2 - 64 / 2);
		tutorialTitle.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 1:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		storeManSprite.DrawSprite(0, 450);
		storeManSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 10; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite1.uvOffsetHandle = i;
				tutorialSprite1.DrawSprite(32 + 64 * i, 520);
			}
		}
		tutorialSprite1.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 2:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 24; i++)
		{
			if (textNum >= i && i < 7)
			{
				tutorialSprite2.uvOffsetHandle = i;
				tutorialSprite2.DrawSprite(32 + 64 * i, 520);
			}
			else if (textNum >= i && i >= 7)
			{
				tutorialSprite3.uvOffsetHandle = (i - 7);
				tutorialSprite3.DrawSprite(32 + 64 * (i - 7), 584);
			}
		}
		tutorialSprite2.Draw();
		tutorialSprite3.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 3:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		storeManSprite.DrawSprite(0, 450);
		storeManSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 25; i++)
		{
			if (textNum >= i && i < 13)
			{
				tutorialSprite4.uvOffsetHandle = i;
				tutorialSprite4.DrawSprite(32 + 64 * i, 520);
			}
			else if (textNum >= i && i >= 13)
			{
				tutorialSprite5.uvOffsetHandle = (i - 13);
				tutorialSprite5.DrawSprite(32 + 64 * (i - 13), 584);
			}
		}
		tutorialSprite4.Draw();
		tutorialSprite5.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}



		break;
	case 4:
		GameSoundMgr::get()->PlayTutorialBGM();

		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 39; i++)
		{
			if (textNum >= i && i < 8)
			{
				tutorialSprite6.uvOffsetHandle = i;
				tutorialSprite6.DrawSprite(32 + 64 * i, 520);
			}
			else if (textNum >= i && i < 26)
			{
				tutorialSprite7.uvOffsetHandle = (i - 8);
				tutorialSprite7.DrawSprite(32 + 64 * (i - 8), 584);
			}
			else if (textNum >= i && i >= 26)
			{
				tutorialSprite8.uvOffsetHandle = (i - 26);
				tutorialSprite8.DrawSprite(32 + 64 * (i - 26), 648);
			}
		}
		tutorialSprite6.Draw();
		tutorialSprite7.Draw();
		tutorialSprite8.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 5:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 24; i++)
		{
			if (textNum >= i && i < 8)
			{
				tutorialSprite9.uvOffsetHandle = i;
				tutorialSprite9.DrawSprite(32 + 64 * i, 520);
			}
			else if (textNum >= i && i >= 8)
			{
				tutorialSprite10.uvOffsetHandle = (i - 8);
				tutorialSprite10.DrawSprite(32 + 64 * (i - 8), 584);
			}
		}
		tutorialSprite9.Draw();
		tutorialSprite10.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 6:
		GameSoundMgr::get()->PlayTutorialBGM();
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 29; i++)
		{
			if (textNum >= i && i < 11)
			{
				tutorialSprite11.uvOffsetHandle = i;
				tutorialSprite11.DrawSprite(32 + 64 * i, 520);
			}
			else if (textNum >= i && i >= 11)
			{
				tutorialSprite12.uvOffsetHandle = (i - 11);
				tutorialSprite12.DrawSprite(32 + 64 * (i - 11), 584);
			}
		}
		tutorialSprite11.Draw();
		tutorialSprite12.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 7:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 3; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite13.uvOffsetHandle = i;
				tutorialSprite13.DrawSprite(32 + 64 * i, 520);
			}
		}
		tutorialSprite13.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 8:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_Y) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_Y) == 0))
		{
			tutorialNum++;
		}
		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(145, 570, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		tutorialSprite23.DrawSprite(94,500);
		tutorialSprite23.Draw();
		break;
	case 9:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 3; i++)
		{
			tutorialSprite13.uvOffsetHandle = i;
			tutorialSprite13.DrawSprite(32 + 64 * i, 520);
		}
		for (int i = 0; i < 3; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite14.uvOffsetHandle = i;
				tutorialSprite14.DrawSprite(224 + 64 * i, 520);
			}
		}
		tutorialSprite13.Draw();
		tutorialSprite14.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 10:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_Y) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_Y) == 0))
		{
			tutorialNum++;
		}
		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(145, 545, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		tutorialSprite23.DrawSprite(94, 500);
		tutorialSprite23.Draw();
		break;

	case 11:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 3; i++)
		{
			tutorialSprite13.uvOffsetHandle = i;
			tutorialSprite13.DrawSprite(32 + 64 * i, 520);
		}
		for (int i = 0; i < 3; i++)
		{
			tutorialSprite14.uvOffsetHandle = i;
			tutorialSprite14.DrawSprite(224 + 64 * i, 520);
		}

		for (int i = 0; i < 3; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite15.uvOffsetHandle = i;
				tutorialSprite15.DrawSprite(416 + 64 * i, 520);
			}
		}
		tutorialSprite13.Draw();
		tutorialSprite14.Draw();
		tutorialSprite15.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 12:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_Y) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_Y) == 0))
		{
			tutorialNum++;
		}
		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(145, 545, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		tutorialSprite23.DrawSprite(94, 500);
		tutorialSprite23.Draw();
		break;
	case 13:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 3; i++)
		{
			tutorialSprite13.uvOffsetHandle = i;
			tutorialSprite13.DrawSprite(32 + 64 * i, 520);
		}
		for (int i = 0; i < 3; i++)
		{
			tutorialSprite14.uvOffsetHandle = i;
			tutorialSprite14.DrawSprite(224 + 64 * i, 520);
		}
		for (int i = 0; i < 3; i++)
		{
			tutorialSprite15.uvOffsetHandle = i;
			tutorialSprite15.DrawSprite(416 + 64 * i, 520);
		}

		for (int i = 0; i < 3; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite16.uvOffsetHandle = i;
				tutorialSprite16.DrawSprite(608 + 64 * i, 520);
			}
		}
		tutorialSprite13.Draw();
		tutorialSprite14.Draw();
		tutorialSprite15.Draw();
		tutorialSprite16.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;

	case 14:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_X) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_X) == 0))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(25, 570, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		tutorialSprite25.DrawSprite(10, 500);
		tutorialSprite25.Draw();
		break;
	case 15:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		//headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 3; i++)
		{
			tutorialSprite13.uvOffsetHandle = i;
			tutorialSprite13.DrawSprite(32 + 64 * i, 520);
		}
		for (int i = 0; i < 3; i++)
		{
			tutorialSprite14.uvOffsetHandle = i;
			tutorialSprite14.DrawSprite(224 + 64 * i, 520);
		}
		for (int i = 0; i < 3; i++)
		{
			tutorialSprite15.uvOffsetHandle = i;
			tutorialSprite15.DrawSprite(416 + 64 * i, 520);
		}
		for (int i = 0; i < 3; i++)
		{
			tutorialSprite16.uvOffsetHandle = i;
			tutorialSprite16.DrawSprite(608 + 64 * i, 520);
		}

		for (int i = 0; i < 14; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite17.uvOffsetHandle = i;
				tutorialSprite17.DrawSprite(32 + 64 * i, 584);
			}
		}
		tutorialSprite13.Draw();
		tutorialSprite14.Draw();
		tutorialSprite15.Draw();
		tutorialSprite16.Draw();
		tutorialSprite17.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;


	case 16:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 3; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite13.uvOffsetHandle = i;
				tutorialSprite13.DrawSprite(32 + 64 * i, 520);
			}
		}
		tutorialSprite13.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;

	case 17:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_B) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_B) == 0))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(265, 570, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		tutorialSprite24.DrawSprite(200, 500);
		tutorialSprite24.Draw();
		break;
	case 18:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 3; i++)
		{
			tutorialSprite13.uvOffsetHandle = i;
			tutorialSprite13.DrawSprite(32 + 64 * i, 520);
		}
		tutorialSprite13.Draw();

		for (int i = 0; i < 16; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite18.uvOffsetHandle = i;
				tutorialSprite18.DrawSprite(224 + 64 * i, 520);
			}
		}
		tutorialSprite18.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;

	case 19:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_X) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_X) == 0))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(25, 570, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		tutorialSprite25.DrawSprite(10, 500);
		tutorialSprite25.Draw();
		break;
	case 20:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		//headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 3; i++)
		{
			tutorialSprite13.uvOffsetHandle = i;
			tutorialSprite13.DrawSprite(32 + 64 * i, 520);
		}
		tutorialSprite13.Draw();

		for (int i = 0; i < 16; i++)
		{
			tutorialSprite18.uvOffsetHandle = i;
			tutorialSprite18.DrawSprite(224 + 64 * i, 520);
		}
		tutorialSprite18.Draw();

		for (int i = 0; i < 14; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite17.uvOffsetHandle = i;
				tutorialSprite17.DrawSprite(32 + 64 * i, 584);
			}
		}
		tutorialSprite17.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 21:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 3; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite13.uvOffsetHandle = i;
				tutorialSprite13.DrawSprite(32 + 64 * i, 520);
			}
		}
		tutorialSprite13.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 22:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 3; i++)
		{
			tutorialSprite13.uvOffsetHandle = i;
			tutorialSprite13.DrawSprite(32 + 64 * i, 520);
		}
		tutorialSprite13.Draw();

		for (int i = 0; i < 8; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite19.uvOffsetHandle = i;
				tutorialSprite19.DrawSprite(224 + 64 * i, 520);
			}
		}
		tutorialSprite19.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 23:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_X) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_X) == 0))
		{
			tutorialNum++;
		}

		headMan->heads.begin()->get()->isMostFront = true;

		hirightSprite1.DrawSprite(25, 545, { 0,0,0,0.8f });
		hirightSprite1.Draw();

		tutorialSprite25.DrawSprite(10, 500);
		tutorialSprite25.Draw();
		break;
	case 24:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		//headMan->heads.begin()->get()->isMostFront = false;

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 3; i++)
		{
			tutorialSprite13.uvOffsetHandle = i;
			tutorialSprite13.DrawSprite(32 + 64 * i, 520);
		}
		tutorialSprite13.Draw();

		for (int i = 0; i < 8; i++)
		{
			tutorialSprite19.uvOffsetHandle = i;
			tutorialSprite19.DrawSprite(224 + 64 * i, 520);
		}
		tutorialSprite19.Draw();

		for (int i = 0; i < 6; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite20.uvOffsetHandle = i;
				tutorialSprite20.DrawSprite(32 + 64 * i, 584);
			}
		}
		tutorialSprite20.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 25:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 14; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite21.uvOffsetHandle = i;
				tutorialSprite21.DrawSprite(32 + 64 * i, 520);
			}
		}
		tutorialSprite21.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 26:
		if ((Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) && !Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A) == 0))
		{
			tutorialNum++;
			textNum = 0;
		}

		hagesiSprite.DrawSprite(0, 450);
		hagesiSprite.Draw();

		textSpeed++;
		if (textSpeed > MAX_TEXT_SPEED)
		{
			textNum++;
			textSpeed = 0;
		}

		for (int i = 0; i < 17; i++)
		{
			if (textNum >= i)
			{
				tutorialSprite22.uvOffsetHandle = i;
				tutorialSprite22.DrawSprite(32 + 64 * i, 520);
			}
		}
		tutorialSprite22.Draw();

		if (aTimer < 60)
		{
			tutorialSprite27.DrawSprite(1280 - 64, 720 - 64);
			tutorialSprite27.Draw();
		}

		break;
	case 27:
		mSceneChanger->ChangeScene(eScene_Game);

		break;
	default:
		break;
	}
}

void TutorialScene::DrawImgui()
{
}
