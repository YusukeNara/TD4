#pragma once
#include"Head.h"

class ScoreManager
{
private:
	//最初に10票ずつ計算する用
	static const int FIRST_NUM_MAX = 10;
	//最大レビュー
	static const int MAX_REVIEW = 5;
	//基本のスコア
	static const int DEFO_SCORE = 800;
private:
	//総合的なスコア
	static float totalScore;
	//総合的なレビュー
	static float totalReview;

	//一時保存用のレビュー
	float oldReview;
	//レビューの計算用
	std::vector<float> reviewNum;

	//はげのスコア
	float SKINHEAD_SCORE_NUM = 5;
	//一本はげ
	float THINNING_SCORE_NUM = 4;
	//アフロ
	float AFROHEAD_SCORE_NUM = 3;
private:
	//最初のNumセット
	void FirstNumSet();
public:
	//初期化処理
	void Initialize();
	//更新処理
	void Update(Head *heads, const CheraType cheraType);
	//スコアの参照
	static const float &GetScore() { return totalScore; }
	//レビューの参照
	static const float &GetReview() { return totalReview; }
};

