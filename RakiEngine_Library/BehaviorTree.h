#pragma once
#include "AI_BehaviorBaseNode.h"

//ビヘイビアツリークラス
class AI_BehaviorTree
{
public:
	//ルートノードポインタ
	BehaviorBaseNode* rootNode;

public:
	//初期化（最初に実行するノードを選択）
	void Init(BehaviorBaseNode* firstNode);

	//ノードの推論
	void Inference();

	//ルートノードの実行
	void Run();
	
	//先頭ノード
	BehaviorBaseNode *firstNode;

	//ツリー名
	std::string treeName;

};