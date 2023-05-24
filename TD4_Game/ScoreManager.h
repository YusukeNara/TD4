#pragma once
#include<time.h>
#include<Sprite.h>

#include"Head.h"

class ScoreManager
{
private:
	//最初に10票ずつ計算する用
	static const int FIRST_NUM_MAX = 10;
	//最大レビュー
	static const int MAX_REVIEW = 5;
	//はげの基本レビュー
	static const int DEFO_HAGEHEAD_REVIEW = 3;
	//一本はげの基本レビュー
	static const int DEFO_LIGHTHEAD_REVIEW = 3;
	//アフロの基本レビュー
	static const int DEFO_AFROHEAD_REVIEW = 3;
	//いい選択（ゲーム的には悪い選択）(基本レビューに足す値)
	static const int GOOD_ACTION = 1;
	//悪い選択(ゲーム的には良い選択)（基本レビューから引く値）
	static const int BAD_ACTION = 2;
	//基本のスコア
	static const int DEFO_SCORE = 800;
private:
	//スプライト
	Sprite spriteScore;
	Sprite spriteReview;

	//総合的なスコア
	static float totalScore;
	//総合的なレビュー
	static float totalReview;

	//一時保存用のスコア
	std::vector<float> scoreNum;

	//一時保存用のレビュー
	float oldReview;
	//増減させる為のレビュー
	float reduceReview;
	//レビューの計算用
	std::vector<float> reviewNum;

	//時間計測
	time_t beginTime;
	time_t endTime;
	double elpsedTime;

	//はげのスコア
	float skinheadReview;
	//一本はげのスコア
	float thinningReview;
	//アフロのスコア
	float afroheadReview;

	//スコアが足される表示のためのフラグ
	bool scorePrintFlag;
	//スコアの位置
	XMFLOAT2 scorePos;
private:
	//最初のNumセット
	void FirstNumSet();
public:
	//初期化処理
	void Initialize();
	//更新処理
	void Update(Head *heads, const CheraType cheraType, const ItemType itemType);
	//描画
	void Draw();
	//スコアの参照
	static const float &GetScore() { return totalScore; }
	//レビューの参照
	static const float &GetReview() { return totalReview; }
};

