# アプリケーション用SceneManagerの使い方

- [該当するファイル](#該当するファイル)
- [概要](#概要)
- [クラスの説明](#クラスの説明)
- [新規シーン実装方法](#新規シーン実装方法)
- [その他](#その他)

## 該当するファイル

- Task（ヘッダーのみ）
- BaseScene
- ISceneChanger
- SceneManager

予め実装済のシーンクラス
- TitleScene
- GameScene

## 概要

エンジン側の機能としてシーンクラスを作るのが面倒（惰性）なので  
アプリケーション側にシーン関連機能を実装しています。  
考え方はステートパターンに近いです。  
継承さえわかればなんとかなります。  

## クラスの説明

- Task.h

シーン関連機能すべての基底クラスです。とくに触る必要はないです。  
（てかここをいじって欲しい機能を実装すると派生クラスに大きく影響します。やめとけ）

- BaseScene

シーンの基底クラスです。アプリケーション側で新たにシーンを作成する場合  
このクラスから派生クラスを作ります。  
このクラスそのものをいじる必要は基本ありません。  

ただし、全シーン共通で使用するクラスなどはここで宣言する必要があるかもしれません。  
この辺のクラス設計で悩んだらエンジン担当に相談してください。  

メンバ関数は以下
```cpp

class BaseScene : public Task {

    //template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

protected:
    
    //シーン切り替えに使用するインターフェイスクラス。
    //使い方はISceneChangerで
    ISceneChanger *mSceneChanger = nullptr;

public:
    //シーン切り替え時に実行される（シーンは動的生成されるため）
    //ここにロードなどを入れると良い。
    BaseScene(ISceneChanger *changer);
    BaseScene() {};
    virtual ~BaseScene() {
    }

    //ゲームシーンの初期化を行う関数。シーン開始1F目にのみ実行される。
    //変数の初期化に使うと良い。
    virtual void Initialize() override {} 

    //シーンが終了するときに呼ばれる。
    //でもデストラクタ使うからこっちは使わない。
    virtual void Finalize() override {}

    //シーンの更新処理。毎フレーム実行される。
    virtual void Update() override {}

    //シーンの描画処理。こっちは3Dの描画をすること。
    //理由は後述。
    virtual void Draw() override{}

    //シーンの更新処理。こっちはSpriteなどの2D描画を実装すること。
    virtual void Draw2D() override{}

};

```

- ISceneChanger

シーンの切り替えに使用するクラス。BaseSceneクラスを継承した場合必ずメンバ変数に付いてくる。  
シーンを切り替えるときはBasceSceneのメンバ変数にあるこのクラスのChangeSceneを呼び出す。

- SceneManager

実際のシーン管理をしているクラス。  
新しいシーンを作る場合、このクラスに処理を書く必要がある。  
詳しくは下の例を参照

```cpp 

// SceneManager.h

class SceneManager : public ISceneChanger ,Task
{
private:

    BaseScene *nowScene;
	eScene mNextScene;    //次のシーン管理変数

public:
    //コンスタラクタ
    SceneManager();
    ~SceneManager() override;
    void Initialize() override;//初期化
    void Finalize() override;//終了処理
    void Update() override;//更新
    void Draw() override;//描画
    void Draw2D() override;

    // 引数 nextScene にシーンを変更する
    void ChangeScene(eScene NextScene) override;

};

```
``` cpp

//SceneManager.cpp

SceneManager::SceneManager() :mNextScene(eScene_None) {

    //タイトルシーンのインスタンス生成
    nowScene = (BaseScene *) new TitleScene(this);
    Initialize();
}

SceneManager::~SceneManager()
{
    delete nowScene;
}

void SceneManager::Initialize()
{
    //シーンの初期化
	nowScene->Initialize();
}

void SceneManager::Finalize()
{
    nowScene->Finalize();
}

void SceneManager::Update()
{
    //ChangeSceneが実行されていると、対応したシーンに切り替わる
    if (mNextScene != eScene_None) {
        delete nowScene;
        nowScene = nullptr;
        
        switch (mNextScene) {       //シーンによって処理を分岐
        case eScene_Title:        //次の画面がタイトルなら
        //タイトルシーンを生成する。
            nowScene = (BaseScene*) new TitleScene(this);
            break;//以下略
        case eScene_Game:
            nowScene = (BaseScene*) new GameScene(this);
            break;
        }
        mNextScene = eScene_None;    //次のシーン情報をクリア
    }

    nowScene->Update(); //シーンの更新
}

void SceneManager::Draw()
{
    //現在シーンの描画処理を実行
    nowScene->Draw();
}

void SceneManager::Draw2D()
{
    //2D描画処理を実行
    nowScene->Draw2D();
}

void SceneManager::ChangeScene(eScene NextScene)
{
    //次のシーン番号をセット
    mNextScene = NextScene;
}

```

## 新規シーン実装方法

1. BaseSceneを継承したシーンクラスを作成する

```cpp

//newScene.h

#include "BaseScene"

class newScene : public BaseScene
{
public:
    newScene(ISceneChanger *changer);
    //デストラクタもきちんと書いてね
    ~newScene() override;

    void Initialize() override;    //初期化処理をオーバーライド。
    void Finalize() override ;   //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;          //描画処理をオーバーライド。
    void Draw2D() override;

private:
    //各シーンで使用するメンバ変数はここに
    Sprite testsp;
    std::shared_ptr<Object3d> testobject;
}

```



2. ISceneChangerのenumに、新規シーンの番号を割り当てる

```cpp
//ISceneChanger.h

typedef enum {
    eScene_Title,   //タイトル画面
    eScene_Game,    //ゲーム画面

    //新規シーンの変数名をここに記述
    eScene_newScene,

    //これは消さないように
    eScene_None,    //無し
} eScene;

//シーンを変更するためのインターフェイスクラス
class ISceneChanger {
public:
    virtual ~ISceneChanger() = 0;
    virtual void ChangeScene(eScene NextScene) = 0;//指定シーンに変更する
};

```


3. SceneManagerにシーンの追加処理を書く

```cpp
//SceneManager.cpp

void SceneManager::Update()
{
    if (mNextScene != eScene_None) {
        delete nowScene;
        nowScene = nullptr;
        
        switch (mNextScene) {
        case eScene_Title:
            nowScene = (BaseScene*) new TitleScene(this);
            break;
        case eScene_Game:
            nowScene = (BaseScene*) new GameScene(this);
            break;


        //先程enumで宣言した変数名でswitchに分岐を追加
        case eScene_newScene:

        //nowSceneにnewで新たなシーンを入れる
        nowScene = (BaseScene*) new GameScene(this);

        //これ忘れるとバグの原因になる
        break;

        }
        mNextScene = eScene_None;
    }

    nowScene->Update();
}

```

## その他

一つ確かなことは、**安易に基底クラス（BaseScene,Task）をいじってはならない**  
基底クラスを変更すると、Scene関連全てに影響が出ます。  
大体のことは基底クラスをいじらなくても解決できます。  
もしわからなくなったら気軽にエンジン担当に相談してください。




