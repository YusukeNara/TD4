#pragma once
typedef enum {
    eScene_Title,   //タイトル画面
    eScene_Game,    //ゲーム画面
    eScene_None,    //無し
} eScene;

//シーンを変更するためのインターフェイスクラス
class ISceneChanger {
public:
    virtual ~ISceneChanger() = 0;
    virtual void ChangeScene(eScene NextScene) = 0;//指定シーンに変更する
};

