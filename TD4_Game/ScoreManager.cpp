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
	reviewPos = { 32,256 };
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
	spriteReview.CreateAndSetDivisionUVOffsets(3, 3, 1, 128, 128, TexManager::LoadTexture("Resources/star.png"));
	spriteLife.Create(TexManager::LoadTexture("Resources/life.png"));
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
		reviewPos = { 32,256 };
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
	if (totalReview <= 5 && totalReview >= 4.75f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 4.25f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 1;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 3.75f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 3.25f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 1;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 2.75f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 2.25f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 1;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 1.75f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 1.25f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 1;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 0.75f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 0.25f)
	{
		spriteReview.uvOffsetHandle = 1;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else
	{
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
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
	if (totalScore >= 1000000)
	{
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(1280.f - 384.f, 720.f - 64.f, 1280.0f - 320.f, 720.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(1280.f - 320.f, 720.f - 64.f, 1280.0f - 256.f, 720.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(1280.f - 256.f, 720.f - 64.f, 1280.0f - 192.f, 720.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(1280.f - 192.f, 720.f - 64.f, 1280.0f - 128.f, 720.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.0f - 64.f, 720.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.0f - 0.f, 720.f);
	}
	else if (totalScore >= 100000)
	{
		spriteScore.uvOffsetHandle = (int)totalScore / 100000 % 10;
		spriteScore.DrawExtendSprite(1280.f - 384.f, 720.f - 64.f, 1280.0f - 320.f, 720.f);
		spriteScore.uvOffsetHandle = (int)totalScore / 10000 % 10;
		spriteScore.DrawExtendSprite(1280.f - 320.f, 720.f - 64.f, 1280.0f - 256.f, 720.f);
		spriteScore.uvOffsetHandle = (int)totalScore / 1000 % 10;
		spriteScore.DrawExtendSprite(1280.f - 256.f, 720.f - 64.f, 1280.0f - 192.f, 720.f);
		spriteScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteScore.DrawExtendSprite(1280.f - 192.f, 720.f - 64.f, 1280.0f - 128.f, 720.f);
		spriteScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteScore.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.0f - 64.f, 720.f);
		spriteScore.uvOffsetHandle = (int)totalScore % 10;
		spriteScore.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.0f - 0.f, 720.f);
	}
	else if (totalScore >= 10000)
	{
		spriteScore.uvOffsetHandle = (int)totalScore / 10000 % 10;
		spriteScore.DrawExtendSprite(1280.f - 320.f, 720.f - 64.f, 1280.0f - 256.f, 720.f);
		spriteScore.uvOffsetHandle = (int)totalScore / 1000 % 10;
		spriteScore.DrawExtendSprite(1280.f - 256.f, 720.f - 64.f, 1280.0f - 192.f, 720.f);
		spriteScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteScore.DrawExtendSprite(1280.f - 192.f, 720.f - 64.f, 1280.0f - 128.f, 720.f);
		spriteScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteScore.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.0f - 64.f, 720.f);
		spriteScore.uvOffsetHandle = (int)totalScore % 10;
		spriteScore.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.0f - 0.f, 720.f);
	}
	else if (totalScore >= 1000)
	{
		spriteScore.uvOffsetHandle = (int)totalScore / 1000 % 10;
		spriteScore.DrawExtendSprite(1280.f - 256.f, 720.f - 64.f, 1280.0f - 192.f, 720.f);
		spriteScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteScore.DrawExtendSprite(1280.f - 192.f, 720.f - 64.f, 1280.0f - 128.f, 720.f);
		spriteScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteScore.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.0f - 64.f, 720.f);
		spriteScore.uvOffsetHandle = (int)totalScore % 10;
		spriteScore.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.0f - 0.f, 720.f);
	}
	else
	{
		spriteScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteScore.DrawExtendSprite(1280.f - 192.f, 720.f - 64.f, 1280.0f - 128.f, 720.f);
		spriteScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteScore.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.0f - 64.f, 720.f);
		spriteScore.uvOffsetHandle = (int)totalScore % 10;
		spriteScore.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.0f - 0.f, 720.f);
	}

	//捌いた人数
	if (handleNum >= 1000)
	{
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 64.f, HANDLE_POS.y, HANDLE_POS.x + 128.f, HANDLE_POS.y + 64.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 0.f, HANDLE_POS.y, HANDLE_POS.x + 64.f, HANDLE_POS.y + 64.f);
	}
	else if (handleNum >= 100)
	{
		spriteScore.uvOffsetHandle = (int)handleNum % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
		spriteScore.uvOffsetHandle = ((int)handleNum / 10) % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 64.f, HANDLE_POS.y, HANDLE_POS.x + 128.f, HANDLE_POS.y + 64.f);
		spriteScore.uvOffsetHandle = (int)handleNum / 100 % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 0.f, HANDLE_POS.y, HANDLE_POS.x + 64.f, HANDLE_POS.y + 64.f);
	}
	else if (handleNum >= 10)
	{
		spriteScore.uvOffsetHandle = (int)handleNum % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
		spriteScore.uvOffsetHandle = ((int)handleNum / 10) % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 64.f, HANDLE_POS.y, HANDLE_POS.x + 128.f, HANDLE_POS.y + 64.f);
	}
	else
	{
		spriteScore.uvOffsetHandle = (int)handleNum % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
	}

	//体力の表示
	spriteLife.DrawExtendSprite(LIFE_POS.x, LIFE_POS.y, LIFE_POS.x + life, LIFE_POS.y + LIFE_SIZE_Y);

	spriteScore.Draw();
	spriteReview.Draw();
	spriteLife.Draw();
}
