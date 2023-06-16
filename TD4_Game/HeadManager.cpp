#include "HeadManager.h"
#include"HageHead.h"
#include"AfroHead.h"
#include"LightHearHead.h"

HeadManager::HeadManager(ProtoPlayer *player)
{
	this->player = player;
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

	//スコアマネージャー内のlifeにplayerの体力をいれる
	scoreManager->SetLife(player->RetirementMoney);

	//先頭の人の処理が終わったら先頭を消す
	for (int headNum = 0; headNum < heads.size(); headNum++)
	{
		scoreManager->Update(heads[headNum].get(), charaType[0], player->GetItemType());
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

void HeadManager::DrawUI()
{
	scoreManager->Draw();
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

void HeadManager::TutorialInit()
{
	RVector3 easeOffset(0, 0, 100.0f);
	int i = 0;
	for (auto &ep : easepos) {
		ep = easeOffset * float(i);
		ep.z += 100.0f;
		i++;
	}

	TutorialFirstSpawn();

	scoreManager = new ScoreManager();
	scoreManager->TutorialInit();
}

void HeadManager::TutorialUpdate()
{
	if (tutorialNum < 3)
	{
		heads.begin()->get()->waitTime = 0;
		heads.begin()->get()->isMostFront = true;
	}

	//先頭の人の処理が終わったら先頭を消す
	for (int headNum = 0; headNum < 4; headNum++)
	{
		scoreManager->Update(heads[0].get(), charaType[0], player->GetItemType());
		if (heads[0]->isAllMoveFinish)
		{
			PopFront();

			for (auto &h : heads)
			{
				h->ResetFrontEase();
			}
			tutorialNum++;
		}
	}

	//更新処理
	for (auto &h : heads) {
		h->Update();
	}
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

void HeadManager::TutorialFirstSpawn()
{
	std::unique_ptr<Head> head = std::make_unique<Head>();

	head.reset(new AfroHead());
	head->SetPlayer(player);

	head->Init();
	head->pos = easepos[0];
	charaType[0] = CheraType::SkinHead;
	head->ResetFrontEase();
	heads.push_back(std::move(head));

	std::unique_ptr<Head> head2 = std::make_unique<Head>();

	head2.reset(new LightHairHead());
	head2->SetPlayer(player);

	head2->Init();
	head2->pos = easepos[1];
	charaType[1] = CheraType::Afro;
	head2->ResetFrontEase();
	heads.push_back(std::move(head2));

	std::unique_ptr<Head> head3 = std::make_unique<Head>();

	head3.reset(new HageHead());
	head3->SetPlayer(player);

	head3->Init();
	head3->pos = easepos[2];
	charaType[2] = CheraType::SkinHead;
	head3->ResetFrontEase();
	heads.push_back(std::move(head3));

	std::unique_ptr<Head> head4 = std::make_unique<Head>();

	head4.reset(new HageHead());
	head4->SetPlayer(player);

	head4->Init();
	head4->pos = easepos[3];
	charaType[3] = CheraType::SkinHead;
	head4->ResetFrontEase();
	heads.push_back(std::move(head4));
}

Head *HeadManager::HeadSpawn(const int arrayNum)
{
	Head *head;

	//ランダムで頭を生成(全ての出現確率の合計)
	int probability = NY_random::intrand_sl(HAGEHEAD_SPAWN_NUM + LIGHTHEAD_SPAWN_NUM + AFROHEAD_SPAWN_NUM, 1);
	//1 ～ HAGEHEAD_SPAWN_NUMだったらはげ
	if (probability >= 1 && probability <= HAGEHEAD_SPAWN_NUM)
	{
		head = new HageHead();
		head->SetPlayer(player);
		charaType[arrayNum] = CheraType::SkinHead;
	}
	//HAGEHEAD_SPAWN_NUM + 1 ～ HAGEHEAD_SPAWN_NUM + LIGHTHEAD_SPAWN_NUMだったら一本はげ
	else if (probability >= (HAGEHEAD_SPAWN_NUM + 1) && probability <= (HAGEHEAD_SPAWN_NUM + LIGHTHEAD_SPAWN_NUM))
	{
		head = new LightHairHead();
		head->SetPlayer(player);
		charaType[arrayNum] = CheraType::Thinning;
	}
	//HAGEHEAD_SPAWN_NUM + LIGHTHEAD_SPAWN_NUM + 1 ～ HAGEHEAD_SPAWN_NUM + LIGHTHEAD_SPAWN_NUM + AFROHEAD_SPAWN_NUMだったらアフロ
	else if (probability >= ((HAGEHEAD_SPAWN_NUM + LIGHTHEAD_SPAWN_NUM) + 1) &&
		probability <= (HAGEHEAD_SPAWN_NUM + LIGHTHEAD_SPAWN_NUM + AFROHEAD_SPAWN_NUM))
	{
		head = new AfroHead();
		head->SetPlayer(player);
		charaType[arrayNum] = CheraType::Afro;
	}
	else
	{
		head = new HageHead();
		head->SetPlayer(player);
		charaType[arrayNum] = CheraType::SkinHead;
	}
	return head;
}