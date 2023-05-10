# ImGuiの使い方

- [概要](#概要)
- [使い方](#使い方)
  - [新規ウィンドウ](#新規ウィンドウ)
  - [ウィンドウを閉じる](#ウィンドウを閉じる)
- [ImGui関数リファレンス](#imgui関数リファレンス)
  - [テキスト](#テキストの表示)
  - [ボタン](#ボタン関連)
  - [スライダー](#スライダー)

# 概要

ウィンドウの生成、破棄だけエンジンでラップしてます。  
ImGuiの関数は、namespace ImGuiにあります。  
パラメータ調整だけでなく、テキスト表示もあるので、デバッグテキストがほしいとは思いますが  
RakiEngineではImGuiに慣れてください。  
（デバッグテキストの必要性を感じないんや...）

詳しく知りたい人はこちらのサイトがおすすめ  

**OpenGLやDirectXなGUIにimguiが最強すぎる**  
https://qiita.com/Ushio/items/446d78c881334919e156



# 使い方

ウィンドウの表示、破棄はラッパークラス **Raki_ImguiMgr** がします。  
なのでこの２つを覚えれば大丈夫です。

## 新規ウィンドウ

```cpp
#include <Raki_ImguiMgr.h>

//StartDrawImGui(ウィンドウの名前,ウィンドウ横幅,ウィンドウ縦幅)
myImgui::StartDrawImGui("window", 100, 100);

```

## ウィンドウを閉じる

```cpp
#include <Raki_ImguiMgr.h>

myImgui::EndDrawImGui();

```

## 実際の使い方

```cpp
#include <Raki_ImguiMgr.h>

//StartDrawImGui(ウィンドウの名前,ウィンドウ横幅,ウィンドウ縦幅)
myImgui::StartDrawImGui("window", 100, 100);
//ここからImGuiの関数を使う



//ここまでImGuiの関数を使う
myImgui::EndDrawImGui();
//↑を忘れないように

```

ウィンドウを複数枚表示する場合は、ウィンドウの数だけ↑の数を増やします。
```cpp
#include <Raki_ImguiMgr.h>

//ウィンドウ1枚目
myImgui::StartDrawImGui("window1", 100, 100);

myImgui::EndDrawImGui();

// ------------------------二枚目------------------------ //

//ウィンドウ2枚目
myImgui::StartDrawImGui("window2", 100, 100);

myImgui::EndDrawImGui();
```



# ImGui関数リファレンス

ウィンドウに表示する関数は、ImGuiそのままです

## テキストの表示

```cpp

ImGui::Text("write this");

//数値の表示もできる
ImGui::Text("fps: %.2f", getFrameRate());

```

## ボタン関連

普通のボタン

```cpp
//戻り値はbool
ImGui::Button("button 1");

//こんな感じで使える
if(ImGui::Button("button 1")){
    if(isActive){
        isActive = false;
    }
    else{
        isActive = true;
    }
}
```

チェックボックス

``` cpp
bool isEnable;

//CheckBox("チェックボックスの文字",フラグを格納する変数の参照)
ImGui::Checkbox("isEnable", &isEnable);
```

ラジオボタン

```cpp
//有効化されたラジオボタンが数値を入れる変数
int mode;

//有効なラジオボタンに応じて割り当てる数値をenumで定義
enum{
    MODE_1,
    MODE_2,
}

//RadioButton("ラジオボタンの文字",int型変数の参照,有効化されたときに入れる数値)
 ImGui::RadioButton("mode 1", &mode, MODE_1); 
 ImGui::RadioButton("mode 2", &mode, MODE_2);
```

## スライダー

```cpp
//SliderFloat("スライダーの文字列",スライダーの数値を入れる変数の参照,
//             スライダー最小値,スライダー最大値)
ImGui::SliderFloat("slider 1", &slider1, 0.0f, 1.0f);
```