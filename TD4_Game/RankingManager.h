#pragma once

//RakiEngine
#include <Sprite.h>
#include <RVector.h>

//webサーバー関連
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

//TD4Gameのランキング管理システム
//SQLへのデータ送信、データ取得、描画を担当

class RankingManager
{
public:
	RankingManager() = default;
	~RankingManager();

	void Init();

	void Update();

	void Draw2D();

	void DebugDraw();

	//スコアをサーバーに送信（resultSceneのスコア変数が必要）
	void PostScore(int score);
	//スコアデータを取得し、配列に格納
	std::array<int, 5> GetRanking();

	//リザルト表示開始
	void StartDisplayResults();

private:
	template <class T>
	//ランキングデータ取得
	pplx::task<T> Get(const std::wstring& url);
	//ランキングデータ送信
	pplx::task<int> Post(const std::wstring& url, int score);

	//ランキングデータ配列
	static std::array<int, 5> rankingArray;

	//ランキングデータ取得済を判定
	bool isRankingGot = false;
	bool isRankingSend = false;

	web::json::value json;
	web::json::value response;

	//ランキング表示イージング
	std::array<Rv3Ease::Rv3Easing, 5> rankScoreEase;
	//強調表示イージング
	Rv3Ease::Rv3Bezier3 emphasisEase;
	Rv3Ease::Rv3Bezier3 emphasisMove;

	//数値スプライト
	Sprite numSprite;

	//演出開始フラグ
	bool isStartDisplayRank = false;

	//経過フレーム
	int frame = 0;

	//ポストされたスコアの保持
	int userNewScore = 0;

};
