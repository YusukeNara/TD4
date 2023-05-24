#include "ScoreManager.h"
float ScoreManager::totalScore = 0;
float ScoreManager::totalReview = 0;

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
	scorePos = { 0,0 };

	//現在の時間をセット
	beginTime = time(NULL);

	FirstNumSet();

	for (int i = 0; i < reviewNum.size(); i++)
	{
		oldReview += reviewNum[i];
	}

	totalReview = oldReview / reviewNum.size();

	spriteScore.CreateAndSetDivisionUVOffsets(10, 10, 1, 64, 64, TexManager::LoadTexture("Resources/Score.png"));
}

void ScoreManager::Update(Head *heads, CheraType cheraType, ItemType itemType)
{
	if (heads->isAllMoveFinish)
	{
		//経過時間を計測
		endTime = time(NULL);
		elpsedTime = difftime(endTime, beginTime);

		//時間によってレビューを増減
		if (elpsedTime >= 5)
		{
			reduceReview = 5;
		}
		else if (elpsedTime >= 4)
		{
			reduceReview = 4;
		}
		else if (elpsedTime >= 3)
		{
			reduceReview = 3;
		}
		else if (elpsedTime >= 2)
		{
			reduceReview = 2;
		}
		else if (elpsedTime >= 1)
		{
			reduceReview = 1;
		}
		else
		{
			//早く選択出来たときだけレビューを減らす
			reduceReview = -1;
		}

		if (cheraType == CheraType::SkinHead)
		{
			//最悪なことをしたとき
			if (itemType == ItemType::Hand)
			{
				//基礎レビュー
				skinheadReview = DEFO_HAGEHEAD_REVIEW - BAD_ACTION;
				//経過時間分も含める
				skinheadReview += reduceReview;
				if (skinheadReview > 5)
				{
					skinheadReview = 5;
				}
				reviewNum.push_back(skinheadReview);
			}
			//あっていることをしたとき
			if (itemType == ItemType::Clippers || itemType == ItemType::Scissors)
			{
				//基礎レビュー
				skinheadReview = DEFO_HAGEHEAD_REVIEW + GOOD_ACTION;
				//経過時間分も含める
				skinheadReview += reduceReview;
				if (skinheadReview > 5)
				{
					skinheadReview = 5;
				}
				reviewNum.push_back(skinheadReview);
			}

			oldReview += reviewNum[reviewNum.size() - 1];
		}
		if (cheraType == CheraType::Thinning)
		{
			//最悪なことをしたとき
			if (itemType == ItemType::Scissors)
			{
				//基礎レビュー
				thinningReview = DEFO_LIGHTHEAD_REVIEW - BAD_ACTION;
				//経過時間分も含める
				thinningReview += reduceReview;
				if (thinningReview > 5)
				{
					thinningReview = 5;
				}
				reviewNum.push_back(thinningReview);
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
				reviewNum.push_back(thinningReview);
			}

			oldReview += reviewNum[reviewNum.size() - 1];
		}
		if (cheraType == CheraType::Afro)
		{
			//最悪なことをしたとき
			if (itemType == ItemType::Clippers)
			{
				//基礎レビュー
				afroheadReview = DEFO_AFROHEAD_REVIEW - BAD_ACTION;
				//経過時間分も含める
				afroheadReview += reduceReview;
				if (afroheadReview > 5)
				{
					afroheadReview = 5;
				}
				reviewNum.push_back(afroheadReview);
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
				reviewNum.push_back(afroheadReview);
			}

			oldReview += reviewNum[reviewNum.size() - 1];
		}
		//次の人の計測を始める
		beginTime = time(NULL);


		//レビューの計算
		totalReview = oldReview / reviewNum.size();

		//スコアの計算
		scoreNum.push_back(DEFO_SCORE * (MAX_REVIEW - totalReview));
		scorePrintFlag = true;
		totalScore += scoreNum[scoreNum.size() - 1];
	}
	if (scorePrintFlag == true)
	{
		scorePos.y--;

	}
}

void ScoreManager::Draw()
{
	float drawScore = 128;
	
	spriteScore.uvOffsetHandle = (int)drawScore / 100;
	spriteScore.DrawExtendSprite(1280.f - 192.f, 720.f - 64.f, 1280.0f - 128.f, 720.f);
	spriteScore.uvOffsetHandle = ((int)drawScore / 10) % 10;
	spriteScore.DrawExtendSprite(1280.f - 128.f, 720.f - 64.f, 1280.0f - 64.f, 720.f);
	spriteScore.uvOffsetHandle = (int)drawScore % 10;
	spriteScore.DrawExtendSprite(1280.f - 64.f, 720.f - 64.f, 1280.0f - 0.f, 720.f);
	spriteScore.Draw();
	//spriteReview.Draw();
}
