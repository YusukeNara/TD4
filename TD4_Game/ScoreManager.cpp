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

	//現在の時間をセット
	beginTime = time(NULL);

	FirstNumSet();

	for (int i = 0; i < reviewNum.size(); i++)
	{
		oldReview += reviewNum[i];
	}

	totalReview = oldReview / reviewNum.size();
}

void ScoreManager::Update(Head *heads, CheraType cheraType)
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
			//if()
			//基礎レビュー
			skinheadReview = DEFO_HAGEHEAD_REVIEW - BAD_ACTION;
			//経過時間分も含める
			skinheadReview += reduceReview;
			if (skinheadReview > 5)
			{
				skinheadReview = 5;
			}
			reviewNum.push_back(skinheadReview);
			//あっていることをしたとき
			//if()
			//基礎レビュー
			skinheadReview = DEFO_HAGEHEAD_REVIEW + GOOD_ACTION;
			//経過時間分も含める
			skinheadReview += reduceReview;
			if (skinheadReview > 5)
			{
				skinheadReview = 5;
			}
			reviewNum.push_back(skinheadReview);

			oldReview += reviewNum[reviewNum.size() - 1];
		}
		if (cheraType == CheraType::Thinning)
		{
			//最悪なことをしたとき
			//if()
			//基礎レビュー
			thinningReview = DEFO_LIGHTHEAD_REVIEW - BAD_ACTION;
			//経過時間分も含める
			thinningReview += reduceReview;
			if (thinningReview > 5)
			{
				thinningReview = 5;
			}
			reviewNum.push_back(thinningReview);
			//あっていることをしたとき
			//if()
			//基礎レビュー
			thinningReview = DEFO_LIGHTHEAD_REVIEW + GOOD_ACTION;
			//経過時間分も含める
			thinningReview += reduceReview;
			if (thinningReview > 5)
			{
				thinningReview = 5;
			}
			reviewNum.push_back(thinningReview);

			oldReview += reviewNum[reviewNum.size() - 1];
		}
		if (cheraType == CheraType::Afro)
		{
			//最悪なことをしたとき
			//if()
			//基礎レビュー
			afroheadReview = DEFO_AFROHEAD_REVIEW - BAD_ACTION;
			//経過時間分も含める
			afroheadReview += reduceReview;
			if (afroheadReview > 5)
			{
				afroheadReview = 5;
			}
			reviewNum.push_back(afroheadReview);
			//あっていることをしたとき
			//if()
			//基礎レビュー
			afroheadReview = DEFO_AFROHEAD_REVIEW + GOOD_ACTION;
			//経過時間分も含める
			afroheadReview += reduceReview;
			if (afroheadReview > 5)
			{
				afroheadReview = 5;
			}
			reviewNum.push_back(afroheadReview);

			oldReview += reviewNum[reviewNum.size() - 1];
		}
		//次の人の計測を始める
		beginTime = time(NULL);


		//レビューの計算
		totalReview = oldReview / reviewNum.size();

		//スコアの計算
		totalScore += DEFO_SCORE * (MAX_REVIEW - totalReview);
	}
}
