#pragma once
#include "Head.h"
#include "ProtoPlayer.h"
#include"ScoreManager.h"
#include"NY_random.h"

class HeadManager
{
private:
	//表示最大数
	static const int HEAD_DISPLAY_MAX = 5;
	//はげの出現確率
	static const int HAGEHEAD_SPAWN_NUM = 30;
	//一本はげの出現確率
	static const int LIGHTHEAD_SPAWN_NUM = 30;
	//アフロの出現確率
	static const int AFROHEAD_SPAWN_NUM = 30;
public:
	HeadManager(ProtoPlayer* player);
	~HeadManager();
	void Initialize();
	void Update();
	void Draw();
	void DrawUI();
	//先頭の人が何かを判別する
	CheraType GetFrontType();
public:
	const ScoreManager *GetScoreManager() { return scoreManager; }
private:
	//初回スポーン
	void FirstSpawn();
	/// スポーン管理
	Head *HeadSpawn(const int arrayNum);
	//先頭の人を消す
	void PopFront();
private:
	//頭コンテナ
	std::vector<std::unique_ptr<Head>> heads;
	//イージング用座標
	std::array<RVector3, HEAD_DISPLAY_MAX> easepos;
	//属性
	std::array<CheraType, HEAD_DISPLAY_MAX> charaType;

	ProtoPlayer *player;

	ScoreManager *scoreManager;
};

