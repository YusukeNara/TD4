#pragma once
typedef enum {
    eScene_Title,   //タイトル画面
    eScene_Tutorial,   //チュートリアルシーン
    eScene_Game,    //ゲーム画面
    eScene_Result,   //リザルトシーン

    eScene_Debug,   //エンジン側検証用
    eScene_None,    //無し
} eScene;

//シーンを変更するためのインターフェイスクラス
class ISceneChanger {
public:
    virtual ~ISceneChanger() = 0;
    virtual void ChangeScene(eScene NextScene) = 0;//指定シーンに変更する
};

