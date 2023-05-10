# ParticleManager 仕様書

## 概要

パーティクルの生成、更新、描画を担うクラス。\
このクラス自体はエミッターとしての役割をしている。

***
# 目次

- [ParticleManager 仕様書](#particlemanager-仕様書)
  - [概要](#概要)
- [目次](#目次)
  - [初期化](#初期化)
  - [設定構造体によるパラメーター設定、パーティクル生成](#設定構造体によるパラメーター設定パーティクル生成)
  - [更新、描画](#更新描画)
- [高度なパーティクル操作をする場合の使い方](#高度なパーティクル操作をする場合の使い方)
  - [概要](#概要-1)
  - [派生クラスを作成](#派生クラスを作成)
    - [**ParticlePrototype ParticlePrototype::Clone();**](#particleprototype-particleprototypeclone)
    - [**void ParticlePrototype::Init();**](#void-particleprototypeinit)
    - [**void ParticlePrototype::Update();**](#void-particleprototypeupdate)
  - [ParticleManagerに派生クラスを設定、使用する](#particlemanagerに派生クラスを設定使用する)
    - [初期化](#初期化-1)
    - [パーティクル生成、更新](#パーティクル生成更新)
    - [パーティクル描画](#パーティクル描画)
    - [終了処理(補足)](#終了処理補足)
- [備考](#備考)

***

## 初期化

ParticleManagerのインクルードと初期化

```cpp

// インクルード
#include<ParticleManager.h> 

// ParticleManagerのポインタ型変数を宣言
ParticleManager *pmanager; 

// 生成したパーティクルマネージャー（以下、PManager）を初期化
pmanager = ParticleManager::Create();

```

***

## 設定構造体によるパラメーター設定、パーティクル生成

設定構造体を使用して簡単にパーティクルを生成できる。設定構造体の変数は以下の通り。
```cpp

struct ParticleGrainState 
{
private:
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	//起点（パーティクル開始座標）
	RVector3 position;
	//速度（パーティクルの速度）
	RVector3 vel;
	//速度上限
	RVector3 velMax;
	//速度下限
	RVector3 velMin;
	//加速度（パーティクルの加速度、velに毎フレーム加算される）
	RVector3 acc;

	///色関連

	//色
	XMFLOAT4 color_start;
	//色補完終了点
	XMFLOAT4 color_end;

	///アフィン変換

	//スケーリング
	float scale_start;
	float scale_end;
	//回転
	float rot_start;
	float rot_end;
	//寿命
	int aliveTime;
};

```

設定構造体を用いたパーティクル生成方法は以下の通り
```cpp

//設定構造体のインスタンス
ParticleGrainState pgstate{};
//パラメータ設定
pgstate.position    = RVector3(0,0,0);
pgstate.vel         = RVector3(1,1,0);
pgstate.acc         = RVector3(10.1f,0.1f,0.1f);
pgstate.color_start = XMFLOAT4(1,1,1,1);
pgstate.color_end   = XMFLOAT4(0,0,0,0);
pgstate.scale_start = 1.0f;
pgstate.scale_end   = 0.1f;
pgstate.aliveTime   = 60;

//設定構造体からパーティクルを生成
pmanager->Add(pgstate)

```

***

## 更新、描画

更新と描画は以下の通り
```cpp

//更新関数、毎フレーム呼び出すこと
pmanager->Update();


//描画関数、Raki_DX12B::StartDraw()以降に実行すること
//関数の引数は、TexManagerでロードしたテクスチャのハンドル
pmanager->Draw(ptex);


```

***

# 高度なパーティクル操作をする場合の使い方

## 概要

Prototypeパターンにより、ParticlePrototype基底クラスを継承することによって\
パーティクル一粒一粒の動きをソースコードによって定義できるようにしている。\
\
基底クラスには純粋仮想関数のみ存在している。これをオーバーライドする必要がある。\
オーバーライド関数は3つ存在し、それぞれ継承先で定義を記述しなければならない。\
\
各関数の概要を以下に示す

***
## 派生クラスを作成

はじめに、作るパーティクルの派生クラスを定義する
```cpp

#include <ParticleManager.h>

class MyParticleClass : public ParticlePrototype
{
public:
    //コンストラクタ、デストラクタ
    MyParticleClass();
    ~MyParticleClass();

    //純粋仮想関数をオーバーライド
    ParticlePrototype Clone() override;
    void Init() override;
    void Update() override;

    //メンバ変数や、見やすくするために関数とかあればご自由に
}
```

必要な関数は以下の通り

* ParticlePrototype ParticlePrototype::Clone();
* void ParticlePrototype::Init();
* void ParticlePrototype::Update();

### **ParticlePrototype ParticlePrototype::Clone();**
Prototypeパターンを使う上で重要な関数。\
プロトタイプパターンの説明をする場ではないので簡単に説明すると\
プロトタイプ版生成関数 **Prototype_Add()** では、この関数を呼び出してパーティクルを生成する。\
そしてプロトタイプパターンは、すでに生成した派生クラスのインスタンスから新たにインスタンスを作成する。\
そのため、 **Clone()** の関数定義は以下のようになる

```cpp

// MyParticleClass は ParticlePrototype の派生クラス

MyParticleClass::Clone()
{
    return new MyParticleClass();
}
```

### **void ParticlePrototype::Init();**

**Prototype_Add()** を実行時に呼び出される。パーティクルの粒が出現したフレームでのみ実行される\
初期座標を設定したり、各種パラメータの設定、派生クラスで定義された変数の初期化などを行うとよい。

```cpp

void MyParticleClass::Init()
{
    //x10 y10が初期位置
    pos.x = 10.0f;
    pos.y = 10.0f;

    //初期速度を設定
    vel = RVector3(0,1,0);
    //加速度を設定
    acc = RVector3(0,0.1f,0);
}
```

### **void ParticlePrototype::Update();**
こちらは毎フレーム呼び出される。パーティクルがどういった動きをするか、\
パラメータはどう変動するのかはここで定義すること。\
記述された内容は**毎フレーム実行される**ので注意。\
\
現在フレームを取得したい場合 *nowFrame* 変数で取れる。\
ただしこの値を書き換えると、パーティクルが破棄されるタイミングに影響があるので注意[1]


```cpp

void MyParticleClass::Update()
{
    //速度に加速度を加算
    vel += acc;
    //座標に速度を加算
    pos += vel;
}
```

***

## ParticleManagerに派生クラスを設定、使用する

### 初期化

```cpp

//インクルード
#include <ParticleManager.h>
#include <MyParticleClass.h> //自作派生クラス

//パーティクルマネージャー生成
ParticleManager *pmanager = ParticleManager::Create();

//派生クラスのインスタンス生成
MyParticleClass *p = new MyParticleClass();

//作成した派生クラスのインスタンスをパーティクルマネージャーに設定
pmanager->Prototype_Set(p);

```

### パーティクル生成、更新
```cpp

//パーティクルを生成（引数はパーティクル生成数）
pmanager->Prototype_Add(10);

//パーティクル更新（毎フレーム呼ぶこと）
pmanager->Prototype_Update();

```

### パーティクル描画
```cpp

// ----- Raki_DX12Bの描画開始関数群は省略 ----- //

//パーティクル描画(引数はTexManagerに格納されている画像ハンドル)
pmanager->Prototype_Draw(ptex);

```

### 終了処理(補足)
派生クラスをnewで作成したが、これを終了時にdeleteする必要はない。  
理由は、pmanager->Prototype_Set(p) 実行時に、ポインタの所有権は pmanager のメンバ変数  
std::unique_ptr< ParticlePrototype > prototype_ に移行しているため。  
そのため明示的deleteをすると終了時にエラーを吐くのでむしろしないこと。  
  
余談だが、**pmanagerについても、スマートポインタに持たせるのが望ましい。**
***
# 備考

>* わからないことや不具合はこちらに  
>初めて.md形式で記述したので、よくわからないことがあれば奈良まで。  
>このドキュメントのフィードバックもOK(というかくれ)

>* **プロトタイプ型パーティクル生成機能について**  
>一からパーティクルの動きを定義するのはしんどいと分かったので、ある程度動きを定義した  
>クラス、または関数を作成予定。優先タスクにはしないが、早めに欲しかったら言ってもらえると  
>優先順位を切り替えます。  
>こんなのがほしいでもOK。

>* .mdファイルの書き方について   
>参考サイト  
><br>
>チートシート<br>
>https://qiita.com/kamorits/items/6f342da395ad57468ae3<br>
>vscodeでマークダウンを書くときの違い<br>
>https://qiita.com/h-reader/items/82f2bbf219a2d53d1fdb<br>
>VSCode<br>
>https://azure.microsoft.com/ja-jp/products/visual-studio-code/<br>
>VSCodeで.md書くときに役立つ拡張機能<br>
>https://marketplace.visualstudio.com/items?itemName=yzhang.markdown-all-in-one

[1]: 更新関数実行時、ParticleManagerでnowFrameをインクリメントし、endFrameを上回ったタイミングでパーティクルを消去するため

