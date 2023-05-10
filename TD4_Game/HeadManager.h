#pragma once
#include "Head.h"
#include "ProtoPlayer.h"

class HeadManager
{
private:
	//表示最大数
	static const int HEAD_DISPLAY_MAX = 5;
public:
	HeadManager();
	~HeadManager();
	void Initialize();
	void Update();
	void Draw();
	//先頭の人が何かを判別する
	CheraType GetFrontType();
private:
	//初回スポーン
	void FirstSpawn();
	/// スポーン管理
	Head *HeadSpawn(const int arrayNum);
	//先頭の人を消す
	void PopFront();
private:
	//頭コンテナ
	std::vector<std::shared_ptr<Head>> heads;
	//イージング用座標
	std::array<RVector3, HEAD_DISPLAY_MAX> easepos;
	//属性
	std::array<CheraType, HEAD_DISPLAY_MAX> charaType;
};

