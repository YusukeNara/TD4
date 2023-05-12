#include "HeadManager.h"
#include"HageHead.h"
#include"AfroHead.h"
#include"LightHearHead.h"

HeadManager::HeadManager()
{
}

HeadManager::~HeadManager()
{
}

void HeadManager::Initialize()
{
	RVector3 easeOffset(0, 0, 100.f);
	int i = 0;
	for (auto &ep : easepos) {
		ep = easeOffset * float(i);
		i++;
	}

	FirstSpawn();
}

void HeadManager::Update()
{
	//要素数がMAXよりも少ない場合増やす
	while (heads.size() < HEAD_DISPLAY_MAX)
	{
		Head *ptr = HeadSpawn((heads.size() + 1) - 1);

		heads.push_back(std::make_unique<Head>());
		heads[(heads.size() + 1) - 1].reset(ptr);
		heads[(heads.size() + 1) - 1]->Init();
		heads[(heads.size() + 1) - 1]->pos = easepos[(heads.size() + 1) - 1];
	}

	//先頭の人の処理が終わったら先頭を消す
	for (int headNum = 0; headNum < heads.size(); headNum++)
	{
		if (heads[headNum]->isAllMoveFinish)
		{
			PopFront();
		}
	}

	//更新処理
	for (auto &h : heads) {
		h->Update();
	}
}

void HeadManager::Draw()
{
	for (const auto &h : heads) { h->Draw(); }
}

void HeadManager::PopFront()
{
	heads.erase(heads.begin());
	//先頭が変わるので属性も移動させておく
	for (int i = 0; i < HEAD_DISPLAY_MAX - 1; i++) {
		charaType[i] = charaType[i + 1];
	}
	charaType[HEAD_DISPLAY_MAX - 1] = None;
}

CheraType HeadManager::GetFrontType()
{
	return charaType[0];
}

void HeadManager::FirstSpawn()
{
	//5回スポーン、位置設定
	for (int i = 0; i < HEAD_DISPLAY_MAX; i++) {

		Head *ptr = HeadSpawn(i);

		heads.push_back(std::make_unique<Head>());
		heads[i].reset(ptr);
		heads[i]->Init();
		heads[i]->pos = easepos[i];
	}
}

Head *HeadManager::HeadSpawn(const int arrayNum)
{
	Head *head;

	//ランダムで頭を生成
	head = new AfroHead();
	charaType[arrayNum] = CheraType::SkinHead;
	return head;
}