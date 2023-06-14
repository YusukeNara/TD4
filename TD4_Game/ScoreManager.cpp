#include "ScoreManager.h"
float ScoreManager::totalScore = 0;
float ScoreManager::totalReview = 0;
int ScoreManager::handleNum = 0;

void ScoreManager::FirstNumSet()
{
	for (int i = 0; i < FIRST_NUM_MAX; i++)
	{
		reviewNum.push_back(5);
	}
	for (int i = 0; i < FIRST_NUM_MAX; i++)
	{
		reviewNum.push_back(4);
	}
}

void ScoreManager::Initialize()
{
	totalReview = 0;
	oldReview = 0;
	totalScore = 0;
	elpsedTime = 0;
	scorePrintFlag = false;
	scorePos = { 1280,526 };
	scoreA = 1;
	reviewPos = { 32,TOTALREVIEW_POS.y + 128 };
	reviewPrintFlag = false;
	reviewA = 1;

	handleNum = 0;

	//現在の時間をセット
	beginTime = time(NULL);

	FirstNumSet();

	for (const auto &review : reviewNum)
	{
		oldReview += review;
	}

	totalReview = oldReview / reviewNum.size();

	spriteScore.CreateAndSetDivisionUVOffsets(10, 10, 1, 64, 64, TexManager::LoadTexture("Resources/Score.png"));
	spriteTotalScore.CreateAndSetDivisionUVOffsets(10, 10, 1, 64, 64, TexManager::LoadTexture("Resources/Score.png"));
	spriteReview.CreateAndSetDivisionUVOffsets(3, 3, 1, 128, 128, TexManager::LoadTexture("Resources/star.png"));
	spriteTotalReview.CreateAndSetDivisionUVOffsets(3, 3, 1, 128, 128, TexManager::LoadTexture("Resources/star.png"));
	spriteGuest.CreateAndSetDivisionUVOffsets(10, 10, 1, 64, 64, TexManager::LoadTexture("Resources/Score.png"));
	spriteLife.Create(TexManager::LoadTexture("Resources/life.png"));
	spriteScoreTitle.Create(TexManager::LoadTexture("Resources/ScoreTitle.png"));
	spriteTotalReviewTitle.Create(TexManager::LoadTexture("Resources/TotalReview.png"));
	spriteGuestTitle.Create(TexManager::LoadTexture("Resources/Guest.png"));
}

void ScoreManager::Update(Head *heads, CheraType cheraType, ItemType itemType)
{
	if (scorePrintFlag == false)
	{
		scorePos = { 1280,526 };
		scoreA = 1;
	}

	if (reviewPrintFlag == false)
	{
		reviewPos = { 32,TOTALREVIEW_POS.y + 128 };
		reviewA = 1;
	}

	if (heads->isAllMoveFinish)
	{
		//経過時間を計測
		endTime = time(NULL);
		elpsedTime = difftime(endTime, beginTime);

		//時間によってレビューを増減
		if (elpsedTime >= 6)
		{
			reduceReview = 2.5f;
		}
		else if (elpsedTime >= 5)
		{
			reduceReview = 2.0f;
		}
		else if (elpsedTime >= 4)
		{
			reduceReview = 1.5f;
		}
		else if (elpsedTime >= 3)
		{
			reduceReview = 1.0f;
		}
		else if (elpsedTime >= 2)
		{
			reduceReview = -1.0f;
		}
		else
		{
			//早く選択出来たときだけレビューを減らす
			reduceReview = -2.0f;
		}

		//捌いた数を計測する
		handleNum++;

		if (cheraType == CheraType::SkinHead)
		{
			//最悪なことをしたとき
			if (itemType == ItemType::Hand)
			{
				//基礎レビュー
				skinheadReview = DEFO_HAGEHEAD_REVIEW + BAD_ACTION;
				//経過時間分も含める
				skinheadReview += reduceReview;
				if (skinheadReview < 0)
				{
					skinheadReview = 0;
				}
				if (skinheadReview > 5)
				{
					skinheadReview = 5;
				}
				reviewNum.push_back(skinheadReview);
				oneReview = skinheadReview;
			}
			//あっていることをしたとき
			if (itemType == ItemType::Clippers || itemType == ItemType::Scissors)
			{
				//基礎レビュー
				skinheadReview = DEFO_HAGEHEAD_REVIEW + GOOD_ACTION;
				//経過時間分も含める
				skinheadReview += reduceReview;
				if (skinheadReview < 0)
				{
					skinheadReview = 0;
				}
				if (skinheadReview > 5)
				{
					skinheadReview = 5;
				}
				reviewNum.push_back(skinheadReview);
				oneReview = skinheadReview;
			}

			oldReview = 0;
			for (const auto &review : reviewNum)
			{
				oldReview += review;
			}
		}
		if (cheraType == CheraType::Thinning)
		{
			//最悪なことをしたとき
			if (itemType == ItemType::Scissors)
			{
				//基礎レビュー
				thinningReview = DEFO_LIGHTHEAD_REVIEW + BAD_ACTION;
				//経過時間分も含める
				thinningReview += reduceReview;
				if (thinningReview > 5)
				{
					thinningReview = 5;
				}
				if (thinningReview < 0)
				{
					thinningReview = 0;
				}
				reviewNum.push_back(thinningReview);
				oneReview = thinningReview;
			}
			//あっていることをしたとき
			if (itemType == ItemType::Hand || itemType == ItemType::Clippers)
			{
				//基礎レビュー
				thinningReview = DEFO_LIGHTHEAD_REVIEW + GOOD_ACTION;
				//経過時間分も含める
				thinningReview += reduceReview;
				if (thinningReview > 5)
				{
					thinningReview = 5;
				}
				if (thinningReview < 0)
				{
					thinningReview = 0;
				}
				reviewNum.push_back(thinningReview);
				oneReview = thinningReview;
			}
			oldReview = 0;
			for (const auto &review : reviewNum)
			{
				oldReview += review;
			}
		}
		if (cheraType == CheraType::Afro)
		{
			//最悪なことをしたとき
			if (itemType == ItemType::Clippers)
			{
				//基礎レビュー
				afroheadReview = DEFO_AFROHEAD_REVIEW + BAD_ACTION;
				//経過時間分も含める
				afroheadReview += reduceReview;
				if (afroheadReview > 5)
				{
					afroheadReview = 5;
				}
				if (afroheadReview < 0)
				{
					afroheadReview = 0;
				}
				reviewNum.push_back(afroheadReview);
				oneReview = afroheadReview;
			}
			//あっていることをしたとき
			if (itemType == ItemType::Hand || itemType == ItemType::Scissors)
			{
				//基礎レビュー
				afroheadReview = DEFO_AFROHEAD_REVIEW + GOOD_ACTION;
				//経過時間分も含める
				afroheadReview += reduceReview;
				if (afroheadReview > 5)
				{
					afroheadReview = 5;
				}
				if (afroheadReview < 0)
				{
					afroheadReview = 0;
				}
				reviewNum.push_back(afroheadReview);
				oneReview = afroheadReview;
			}

			oldReview = 0;
			for (const auto &review : reviewNum)
			{
				oldReview += review;
			}
		}
		//次の人の計測を始める
		beginTime = time(NULL);


		//レビューの計算
		totalReview = oldReview / reviewNum.size();

		//スコアの計算
		scoreNum = DEFO_SCORE * (MAX_REVIEW - totalReview);
		scorePrintFlag = true;
		reviewPrintFlag = true;
		totalScore += scoreNum;
	}

	if (scorePrintFlag == true)
	{
		scorePos.y += 0.75f;
		scoreA -= 0.005f;
		if (scoreA >= 0 && scoreA <= 0.1f)
		{
			scorePrintFlag = false;
		}
	}

	if (reviewPrintFlag == true)
	{
		reviewPos.y -= 0.75f;
		reviewA -= 0.005f;
		if (reviewA >= 0 && reviewA <= 0.1f)
		{
			reviewPrintFlag = false;
		}
	}
}

void ScoreManager::Draw()
{
	if (reviewPrintFlag == true)
	{
		//一人用のレビュー
		if (oneReview <= 5 && oneReview >= 4.75f)
		{
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x, reviewPos.y, reviewPos.x + 64.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 64.f, reviewPos.y, reviewPos.x + 128.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 128.f, reviewPos.y, reviewPos.x + 192.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 192.f, reviewPos.y, reviewPos.x + 256.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 256.f, reviewPos.y, reviewPos.x + 320.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
		}
		else if (oneReview >= 4.25f)
		{
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x, reviewPos.y, reviewPos.x + 64.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 64.f, reviewPos.y, reviewPos.x + 128.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 128.f, reviewPos.y, reviewPos.x + 192.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 192.f, reviewPos.y, reviewPos.x + 256.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 1;
			spriteReview.DrawExtendSprite(reviewPos.x + 256.f, reviewPos.y, reviewPos.x + 320.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
		}
		else if (oneReview >= 3.75f)
		{
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x, reviewPos.y, reviewPos.x + 64.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 64.f, reviewPos.y, reviewPos.x + 128.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 128.f, reviewPos.y, reviewPos.x + 192.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 192.f, reviewPos.y, reviewPos.x + 256.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 256.f, reviewPos.y, reviewPos.x + 320.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
		}
		else if (oneReview >= 3.25f)
		{
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x, reviewPos.y, reviewPos.x + 64.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 64.f, reviewPos.y, reviewPos.x + 128.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 128.f, reviewPos.y, reviewPos.x + 192.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 1;
			spriteReview.DrawExtendSprite(reviewPos.x + 192.f, reviewPos.y, reviewPos.x + 256.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 256.f, reviewPos.y, reviewPos.x + 320.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
		}
		else if (oneReview >= 2.75f)
		{
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x, reviewPos.y, reviewPos.x + 64.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 64.f, reviewPos.y, reviewPos.x + 128.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 128.f, reviewPos.y, reviewPos.x + 192.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 192.f, reviewPos.y, reviewPos.x + 256.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 256.f, reviewPos.y, reviewPos.x + 320.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
		}
		else if (oneReview >= 2.25f)
		{
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x, reviewPos.y, reviewPos.x + 64.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 64.f, reviewPos.y, reviewPos.x + 128.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 1;
			spriteReview.DrawExtendSprite(reviewPos.x + 128.f, reviewPos.y, reviewPos.x + 192.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 192.f, reviewPos.y, reviewPos.x + 256.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 256.f, reviewPos.y, reviewPos.x + 320.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
		}
		else if (oneReview >= 1.75f)
		{
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x, reviewPos.y, reviewPos.x + 64.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x + 64.f, reviewPos.y, reviewPos.x + 128.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 128.f, reviewPos.y, reviewPos.x + 192.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 192.f, reviewPos.y, reviewPos.x + 256.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 256.f, reviewPos.y, reviewPos.x + 320.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
		}
		else if (oneReview >= 1.25f)
		{
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x, reviewPos.y, reviewPos.x + 64.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 1;
			spriteReview.DrawExtendSprite(reviewPos.x + 64.f, reviewPos.y, reviewPos.x + 128.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 128.f, reviewPos.y, reviewPos.x + 192.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 192.f, reviewPos.y, reviewPos.x + 256.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 256.f, reviewPos.y, reviewPos.x + 320.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
		}
		else if (oneReview >= 0.75f)
		{
			spriteReview.uvOffsetHandle = 2;
			spriteReview.DrawExtendSprite(reviewPos.x, reviewPos.y, reviewPos.x + 64.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 64.f, reviewPos.y, reviewPos.x + 128.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 128.f, reviewPos.y, reviewPos.x + 192.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 192.f, reviewPos.y, reviewPos.x + 256.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 256.f, reviewPos.y, reviewPos.x + 320.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
		}
		else if (oneReview >= 0.25f)
		{
			spriteReview.uvOffsetHandle = 1;
			spriteReview.DrawExtendSprite(reviewPos.x, reviewPos.y, reviewPos.x + 64.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 64.f, reviewPos.y, reviewPos.x + 128.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 128.f, reviewPos.y, reviewPos.x + 192.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 192.f, reviewPos.y, reviewPos.x + 256.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 256.f, reviewPos.y, reviewPos.x + 320.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
		}
		else
		{
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x, reviewPos.y, reviewPos.x + 64.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 64.f, reviewPos.y, reviewPos.x + 128.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 128.f, reviewPos.y, reviewPos.x + 192.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 192.f, reviewPos.y, reviewPos.x + 256.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
			spriteReview.uvOffsetHandle = 0;
			spriteReview.DrawExtendSprite(reviewPos.x + 256.f, reviewPos.y, reviewPos.x + 320.f, reviewPos.y + 64.f , { 1,1,1,reviewA });
		}
	}
	//トータルレビューの表示
	spriteTotalReviewTitle.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y-64.f, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y);

	if (totalReview <= 5 && totalReview >= 4.75f)
	{
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 4.25f)
	{
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 1;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 3.75f)
	{
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 3.25f)
	{
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 1;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 2.75f)
	{
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 2.25f)
	{
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 1;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 1.75f)
	{
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 1.25f)
	{
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 1;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 0.75f)
	{
		spriteTotalReview.uvOffsetHandle = 2;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 0.25f)
	{
		spriteTotalReview.uvOffsetHandle = 1;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else
	{
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteTotalReview.uvOffsetHandle = 0;
		spriteTotalReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}


	//スコアの表示
	if (scorePrintFlag == true)
	{
		if (scoreNum >= 1000)
		{
			spriteScore.uvOffsetHandle = (int)totalScore / 1000 % 10;
			spriteScore.DrawExtendSprite(scorePos.x - 256.f, scorePos.y - 64.f, scorePos.x - 192.f, scorePos.y);
			spriteScore.uvOffsetHandle = (int)scoreNum / 100 % 10;
			spriteScore.DrawExtendSprite(scorePos.x - 192.f, scorePos.y - 64.f, scorePos.x - 128.f, scorePos.y, { 1,1,1,scoreA });
			spriteScore.uvOffsetHandle = ((int)scoreNum / 10) % 10;
			spriteScore.DrawExtendSprite(scorePos.x - 128.f, scorePos.y - 64.f, scorePos.x - 64.f, scorePos.y, { 1,1,1,scoreA });
			spriteScore.uvOffsetHandle = (int)scoreNum % 10;
			spriteScore.DrawExtendSprite(scorePos.x - 64.f, scorePos.y - 64.f, scorePos.x - 0.f, scorePos.y, { 1,1,1,scoreA });
		}
		else if (scoreNum >= 100)
		{
			spriteScore.uvOffsetHandle = (int)scoreNum / 100 % 10;
			spriteScore.DrawExtendSprite(scorePos.x - 192.f, scorePos.y - 64.f, scorePos.x - 128.f, scorePos.y, { 1,1,1,scoreA });
			spriteScore.uvOffsetHandle = ((int)scoreNum / 10) % 10;
			spriteScore.DrawExtendSprite(scorePos.x - 128.f, scorePos.y - 64.f, scorePos.x - 64.f, scorePos.y, { 1,1,1,scoreA });
			spriteScore.uvOffsetHandle = (int)scoreNum % 10;
			spriteScore.DrawExtendSprite(scorePos.x - 64.f, scorePos.y - 64.f, scorePos.x - 0.f, scorePos.y, { 1,1,1,scoreA });
		}
	}
	//トータルのスコアの表示
	spriteScoreTitle.DrawExtendSprite(1280.f - 384.f, 720 - 128.f, 1280.f - 64.f, 720.f - 64.f);
	if (totalScore >= 1000000)
	{
		spriteTotalScore.uvOffsetHandle = 9;
		spriteTotalScore.DrawExtendSprite(1280.f - 384.f, 720.f - 64.f, 1280.0f - 320.f, 720.f);
		spriteTotalScore.uvOffsetHandle = 9;
		spriteTotalScore.DrawExtendSprite(1280.f - 320.f, 720.f - 64.f, 1280.0f - 256.f, 720.f);
		spriteTotalScore.uvOffsetHandle = 9;
		spriteTotalScore.DrawExtendSprite(1280.f - 256.f, 720.f - 64.f, 1280.0f - 192.f, 720.f);
		spriteTotalScore.uvOffsetHandle = 9;
		spriteTotalScore.DrawExtendSprite(1280.f - 192.f, 720.f - 64.f, 1280.0f - 128.f, 720.f);
		spriteTotalScore.uvOffsetHandle = 9;
		spriteTotalScore.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.0f - 64.f, 720.f);
		spriteTotalScore.uvOffsetHandle = 9;
		spriteTotalScore.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.0f - 0.f, 720.f);
	}
	else if (totalScore >= 100000)
	{
		spriteTotalScore.uvOffsetHandle = (int)totalScore / 100000 % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 384.f, 720.f - 64.f, 1280.0f - 320.f, 720.f);
		spriteTotalScore.uvOffsetHandle = (int)totalScore / 10000 % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 320.f, 720.f - 64.f, 1280.0f - 256.f, 720.f);
		spriteTotalScore.uvOffsetHandle = (int)totalScore / 1000 % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 256.f, 720.f - 64.f, 1280.0f - 192.f, 720.f);
		spriteTotalScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 192.f, 720.f - 64.f, 1280.0f - 128.f, 720.f);
		spriteTotalScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.0f - 64.f, 720.f);
		spriteTotalScore.uvOffsetHandle = (int)totalScore % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.0f - 0.f, 720.f);
	}
	else if (totalScore >= 10000)
	{
		spriteTotalScore.uvOffsetHandle = (int)totalScore / 10000 % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 320.f, 720.f - 64.f, 1280.0f - 256.f, 720.f);
		spriteTotalScore.uvOffsetHandle = (int)totalScore / 1000 % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 256.f, 720.f - 64.f, 1280.0f - 192.f, 720.f);
		spriteTotalScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 192.f, 720.f - 64.f, 1280.0f - 128.f, 720.f);
		spriteTotalScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.0f - 64.f, 720.f);
		spriteTotalScore.uvOffsetHandle = (int)totalScore % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.0f - 0.f, 720.f);
	}
	else if (totalScore >= 1000)
	{
		spriteTotalScore.uvOffsetHandle = (int)totalScore / 1000 % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 256.f, 720.f - 64.f, 1280.0f - 192.f, 720.f);
		spriteTotalScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 192.f, 720.f - 64.f, 1280.0f - 128.f, 720.f);
		spriteTotalScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.0f - 64.f, 720.f);
		spriteTotalScore.uvOffsetHandle = (int)totalScore % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.0f - 0.f, 720.f);
	}
	else
	{
		spriteTotalScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 192.f, 720.f - 64.f, 1280.0f - 128.f, 720.f);
		spriteTotalScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.0f - 64.f, 720.f);
		spriteTotalScore.uvOffsetHandle = (int)totalScore % 10;
		spriteTotalScore.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.0f - 0.f, 720.f);
	}

	//捌いた人数
	spriteGuestTitle.DrawExtendSprite(HANDLE_POS.x + 0.f, HANDLE_POS.y - 64.f, HANDLE_POS.x + 320.f, HANDLE_POS.y);

	if (handleNum >= 1000)
	{
		spriteGuest.uvOffsetHandle = 9;
		spriteGuest.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
		spriteGuest.uvOffsetHandle = 9;
		spriteGuest.DrawExtendSprite(HANDLE_POS.x + 64.f, HANDLE_POS.y, HANDLE_POS.x + 128.f, HANDLE_POS.y + 64.f);
		spriteGuest.uvOffsetHandle = 9;
		spriteGuest.DrawExtendSprite(HANDLE_POS.x + 0.f, HANDLE_POS.y, HANDLE_POS.x + 64.f, HANDLE_POS.y + 64.f);
	}
	else if (handleNum >= 100)
	{
		spriteGuest.uvOffsetHandle = (int)handleNum % 10;
		spriteGuest.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
		spriteGuest.uvOffsetHandle = ((int)handleNum / 10) % 10;
		spriteGuest.DrawExtendSprite(HANDLE_POS.x + 64.f, HANDLE_POS.y, HANDLE_POS.x + 128.f, HANDLE_POS.y + 64.f);
		spriteGuest.uvOffsetHandle = (int)handleNum / 100 % 10;
		spriteGuest.DrawExtendSprite(HANDLE_POS.x + 0.f, HANDLE_POS.y, HANDLE_POS.x + 64.f, HANDLE_POS.y + 64.f);
	}
	else if (handleNum >= 10)
	{
		spriteGuest.uvOffsetHandle = (int)handleNum % 10;
		spriteGuest.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
		spriteGuest.uvOffsetHandle = ((int)handleNum / 10) % 10;
		spriteGuest.DrawExtendSprite(HANDLE_POS.x + 64.f, HANDLE_POS.y, HANDLE_POS.x + 128.f, HANDLE_POS.y + 64.f);
	}
	else
	{
		spriteGuest.uvOffsetHandle = (int)handleNum % 10;
		spriteGuest.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
	}

	//体力の表示
	spriteLife.DrawExtendSprite(LIFE_POS.x, LIFE_POS.y, LIFE_POS.x + life, LIFE_POS.y + LIFE_SIZE_Y);

	spriteScore.Draw();
	spriteTotalScore.Draw();
	spriteReview.Draw();
	spriteTotalReview.Draw();
	spriteGuest.Draw();
	spriteLife.Draw();
	spriteScoreTitle.Draw();
	spriteTotalReviewTitle.Draw();
	spriteGuestTitle.Draw();
}
