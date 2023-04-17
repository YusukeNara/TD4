#pragma once
#include <string>

//判定ノード基底クラス
//継承したクラスのポインタ型オブジェクトを使用
class BehaviorJudgeBase
{
public:
	BehaviorJudgeBase() = default;
	virtual ~BehaviorJudgeBase(){};

	//ノードが使用する純粋仮想関数
	//ノード選択前にこのクラスが実行され、子ノードの使用可能かを判定する
	//ノード選択の判定スクリプトは派生クラスで定義
	virtual bool Judge() = 0;

	std::string judgeScriptName;
};