#pragma once

//RakiEngine
#include <Sprite.h>

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

private:
	template <class T>
	//ランキングデータ取得
	pplx::task<T> Get(const std::wstring& url);
	//ランキングデータ送信
	pplx::task<int> Post(const std::wstring& url, int score);

	//ランキングデータ配列
	std::array<int, 5> rankingArray = { 0,0,0,0,0 };

	//ランキングデータ取得済を判定
	bool isRankingGot = false;
	bool isRankingSend = false;

	web::json::value json;
	web::json::value response;

};
