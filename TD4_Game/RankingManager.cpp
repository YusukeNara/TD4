#include "RankingManager.h"

#include <Raki_imguiMgr.h>
#include <algorithm>

std::array<int, 5> RankingManager::rankingArray = { 6,5,4,3,2 };

RankingManager::~RankingManager()
{
}

void RankingManager::Init()
{

    numSprite.CreateAndSetDivisionUVOffsets(10, 10, 1, 64, 64,
        TexManager::LoadTexture("Resources/Score.png"));


    //ランキング表示X基準座標
    const float EASE_LEFT_OFFSET = 750.0f;
    //ランキング表示Y基準座標
    const float EASE_TOP_OFFSET = 160.0f;
    //ランキング表示Y表示間隔
    const float EASE_Y_OFFSET = 72.0f;

    //イージング座標設定
    for (int i = 0; i < rankScoreEase.size(); i++) {
        float ypos = EASE_TOP_OFFSET + (EASE_Y_OFFSET * (i + 1));

        rankScoreEase[i].Init(Rv3Ease::RV3_EASE_TYPE::EASE_CUBIC_INOUT,
            RVector3(1500.f, ypos, 0),
            RVector3(EASE_LEFT_OFFSET, ypos, 0),
            60);
    }

    const float FONT_SIZE = 50.0f;
    //ベジエ曲線イージングを使用したフォント強調表示
    emphasisEase.Init(RVector3(FONT_SIZE, 0, 0),
        RVector3(FONT_SIZE, 0, 0),
        RVector3(FONT_SIZE * 1.2f, 0, 0),
        30,
        Rv3Ease::RV3_EASE_TYPE::EASE_CUBIC_IN);
    const float FONT_MOVE_OFFSET = 5;
    emphasisMove.Init(RVector3(0, 0, 0),
        RVector3(0, 0, 0),
        RVector3(FONT_MOVE_OFFSET, 0, 0),
        30,
        Rv3Ease::RV3_EASE_TYPE::EASE_CUBIC_IN);

}

void RankingManager::Update()
{
    if (isStartDisplayRank) {
        frame++;

        for (int i = 0; i < 5; i++) {
            if (frame % (15 * (i + 1)) == 0) {
                rankScoreEase[i].Play();
            }
            rankScoreEase[i].Update();
        }
    }

    emphasisEase.Update();
    emphasisMove.Update();
    if (emphasisEase.isEnded()) {
        emphasisEase.Reset();
    }
    if (emphasisMove.isEnded()) {
        emphasisMove.Reset();
    }
}


void RankingManager::Draw2D()
{
    if (!isStartDisplayRank) { return; }

    //文字サイズ
    const float FONT_SIZE = 50.f;
    //スコアの表示位置オフセット
    const float SCORE_POS_X_OFFSET = 120.f;

    for (int i = 0; i < 5;i++) {
        //順位
        numSprite.DrawNumSprite(rankScoreEase[i].GetNowpos().x,
            rankScoreEase[i].GetNowpos().y,
            FONT_SIZE,
            FONT_SIZE,
            i + 1);

        //ランキング配列にユーザースコアがある場合、強調表示する
        if (rankingArray[i] == userNewScore) {
            emphasisEase.Play();
            emphasisMove.Play();
            //スコア
            numSprite.DrawNumSprite(rankScoreEase[i].GetNowpos().x + SCORE_POS_X_OFFSET - emphasisMove.nowPos().x,
                rankScoreEase[i].GetNowpos().y - emphasisMove.nowPos().y,
                emphasisEase.nowPos().x,
                emphasisEase.nowPos().x,
                rankingArray[i]);
        }
        else {
            //スコア
            numSprite.DrawNumSprite(rankScoreEase[i].GetNowpos().x + SCORE_POS_X_OFFSET,
                rankScoreEase[i].GetNowpos().y,
                FONT_SIZE,
                FONT_SIZE,
                rankingArray[i]);
        }

    }



    numSprite.Draw();
}

void RankingManager::DebugDraw()
{
    //imguiでランキングデータ表示

    ImguiMgr::Get()->StartDrawImgui("rankingdata", 100, 100);

    int i = 1;

    for (auto& r : rankingArray) {
        ImGui::Text("%d : %d", i, r);
        i++;
    }

    ImguiMgr::Get()->EndDrawImgui();
}


void RankingManager::PostScore(int score)
{

    if (!isRankingSend) {
        //スコア送信
        try
        {
            auto serverStatusCode = Post(L"http://localhost:3000/hage_games/", score).wait();
            //成功
            if (serverStatusCode == 1 || serverStatusCode == 2) {
                isRankingSend = true;
            }

        }
        catch (const std::exception& e)
        {
            std::cout << "RankingManager | Exception Detected : %s" << e.what() << std::endl;
        }
    }

    userNewScore = score;
}

std::array<int, 5> RankingManager::GetRanking()
{
    if (!isRankingGot) {
        bool isNotConnection = false;

        //スコア取得
        try
        {
            auto task = Get<json::value>(L"http://localhost:3000/hage_games/");
            const json::value j = task.get();
            auto array = j.as_array();

            // JSONオブジェクトから必要部分を切り出してint型の配列に代入
            for (int i = 0; i < array.size(); i++)
            {
                rankingArray[i] = array[i].at(U("score")).as_integer();
            }

            isRankingGot = true;
        }
        catch (const std::exception& e)
        {
            //現在格納されているローカルデータでランキング変動を行う
            isNotConnection = true;

            std::cout << "RankingManager | Exception Detected : %s" << e.what() << std::endl;
        }

        if (isNotConnection) {
            //スコア更新できる順位を検索
            auto rankPos = std::ranges::find_if_not(rankingArray, [&](int x) {return x > userNewScore; });
            //更新できる場合
            if (rankPos != rankingArray.end()) {
                std::shift_right(rankPos, rankingArray.end(), 1);
                *rankPos = userNewScore;
            }
        }
    }




    return rankingArray;
}

void RankingManager::StartDisplayResults()
{
    isStartDisplayRank = true;
}

template<class T>
pplx::task<T> RankingManager::Get(const std::wstring& url)
{
    return pplx::create_task([=]
        {
            http_client client(url);
    return client.request(methods::GET); })
        .then([](http_response response)
            {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } });
}

pplx::task<int> RankingManager::Post(const std::wstring& url, int score)
{
    return pplx::create_task([=]
        {
            json::value postData;
    postData[L"score"] = score;

    http_client client(url);
    return client.request(methods::POST, L"", postData.serialize(),
        L"application/json"); })
        .then([](http_response response)
            {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } })
        .then(
            [](json::value json)
            { return json[L"serverStatus"].as_integer(); });
}
