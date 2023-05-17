#include "HeadManager.h"
#include"HageHead.h"
#include"AfroHead.h"
#include"LightHearHead.h"

HeadManager::HeadManager()
{
}

HeadManager::~HeadManager()
{
	delete scoreManager;
	scoreManager = nullptr;
}

void HeadManager::Initialize()
{
	RVector3 easeOffset(0, 0, 100.0f);
	int i = 0;
	for (auto &ep : easepos) {
		ep = easeOffset * float(i);
		ep.z += 100.0f;
		i++;
	}

	FirstSpawn();

	scoreManager = new ScoreManager();
	scoreManager->Initialize();
}

void HeadManager::Update()
{
	heads.begin()->get()->isMostFront = true;
	//要素数がMAXよりも少ない場合増やす
	while (heads.size() < HEAD_DISPLAY_MAX)
	{
		Head *ptr = HeadSpawn((heads.size() + 1) - 1);
		std::unique_ptr<Head> head = std::make_unique<Head>();

		head.reset(HeadSpawn((heads.size() + 1) - 1));

		head->Init();
		head->pos = easepos[(heads.size() + 1) - 1];
		head->ResetFrontEase();
		heads.push_back(std::move(head));
	}

	//先頭の人の処理が終わったら先頭を消す
	for (int headNum = 0; headNum < heads.size(); headNum++)
	{
		scoreManager->Update(heads[headNum].get(), charaType[0]);
		if (heads[headNum]->isAllMoveFinish)
		{
			PopFront();

			for (auto &h : heads)
			{
				h->ResetFrontEase();
			}
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

		std::unique_ptr<Head> head = std::make_unique<Head>();

		head.reset(HeadSpawn(i));

		head->Init();
		head->pos = easepos[i];
		head->ResetFrontEase();
		heads.push_back(std::move(head));
	}
}

Head *HeadManager::HeadSpawn(const int arrayNum)
{
	Head *head;

	//ランダムで頭を生成(1～100)
	int probability = NY_random::intrand_sl(100, 1);
	//1～20だったらはげ
	if (probability >= 1 && probability <= 20)
	{
		head = new HageHead();
		charaType[arrayNum] = CheraType::SkinHead;
	}
	//21～60だったら一本はげ
	else if (probability >= 21 && probability <= 60)
	{
		head = new LightHairHead();
		charaType[arrayNum] = CheraType::Thinning;
	}
	//61～100だったらアフロ
	else if (probability >= 61 && probability <= 100)
	{
		head = new AfroHead();
		charaType[arrayNum] = CheraType::Afro;
	}
	else
	{
		head = new HageHead();
		charaType[arrayNum] = CheraType::SkinHead;
	}
	return head;
}