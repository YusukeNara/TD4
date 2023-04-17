#pragma once
#include "Behavior_ActionBase.h"


#include <string>
#include <vector>

class BehaviorJudgeBase;

//ビヘイビアのノード
//ここにactionBase,JudgeBaseのポインタを入れて実行する
//アプリケーション側ではこいつのポインタにスクリプトを入れて実行する
//無駄が多いから改善の余地あり
class BehaviorBaseNode
{
public:
	//ノード決定ルール
	enum SELECT_RULE
	{
		RULE_RANDOM,	//ランダム
		RULE_PRIORITY,	//優先度
		RULE_ONOFF,		//オンオフ
		RULE_SEAQUENCE,	//シーケンス
	};

	//ノードの種類
	enum NODE_TYPE
	{
		TYPE_EXECUTE,	//実行ノード
		TYPE_SELECTER,	//選択ノード
	};

public:
	BehaviorBaseNode() {};
	~BehaviorBaseNode() {
	};

	/// <summary>
	/// 判定用ノードを生成
	/// </summary>
	/// <param name="nodeName">ノードの名前</param>
	/// <param name="rule">子ノード選択ルール</param>
	/// <param name="judgeObject">ノード選択条件を定義した派生クラスオブジェクト</param>
	void CreateJudgeNode(std::string nodeName, SELECT_RULE rule, BehaviorJudgeBase* judgeObject);

	/// <summary>
	/// 判定ノードに子ノード追加
	/// </summary>
	/// <param name="child"></param>
	void AddjudgeNodeChild(BehaviorBaseNode* child);

	/// <summary>
	/// 実行ノード生成
	/// </summary>
	/// <param name="nodeName">ノードの名前</param>
	/// <param name="actObject">実行する派生クラスオブジェクト</param>
	void CreateActionNode(std::string nodeName, BehaviorActionBase* actObject, BehaviorJudgeBase* judgeObject);

	/// <summary>
	/// ノード推論
	/// <para> 子ノードの判定クラスを実行し、候補となるノードを選択</para>
	/// <para> 候補ノードの中から選択ルールによって選ばれたノードを選択</para>
	/// <para> これをノードの末端まで繰り返す関数</para>
	/// </summary>
	/// <returns>推論の結果から出たルートノード</returns>
	BehaviorBaseNode *Inference();

	/// <summary>
	/// ノード実行
	/// <para>実行クラスの処理を行う</para>
	/// <para>実行クラスがない場合失敗扱いになる</para>
	/// </summary>
	/// <returns>実行クラスの進行状況</returns>
	BehaviorActionBase::ACTION_STATE Run();

	///以下はノード決定用関数
	BehaviorBaseNode *Select_Random(std::vector<BehaviorBaseNode*> *lists);

	BehaviorBaseNode* Select_Priority(std::vector<BehaviorBaseNode*>* lists);


	//デバッグ用
	void DrawNodeInfo(std::vector<BehaviorBaseNode*> editNodes, std::vector<BehaviorActionBase*> actObjects, 
		std::vector<BehaviorJudgeBase*> judgeObjects, bool checkIsDisplay = true);

	void DrawNodeInfo_Child(std::vector<BehaviorBaseNode*> editNodes, std::vector<BehaviorActionBase*> actObjects, std::vector<BehaviorJudgeBase*> judgeObjects);

	void DrawNodeInfo_withEditor(std::vector<BehaviorBaseNode*> editNodes, std::vector<BehaviorActionBase*> actObjects, std::vector<BehaviorJudgeBase*> judgeObjects);

public:
	//ノードの名前
	std::string		nodeName;
	//ノードの種類
	NODE_TYPE		type;
	//ノード選択ルール
	SELECT_RULE		rule;
	//親ノード
	BehaviorBaseNode*				parent		= nullptr;
	//子ノード配列
	std::vector<BehaviorBaseNode*>	childs;
	//判定クラス
	BehaviorJudgeBase*				judgeObject = nullptr;
	//実行クラス
	BehaviorActionBase*				actObject	= nullptr;

	//ノード優先度
	unsigned int	priority;

private:
	//エディター専用
	bool isDisplay = false;
	bool isChangeChilds = false;
	bool isChangeScripts = false;


	friend class BehaviorEditor;
};


enum class NODE_STATUS
{
	RUNNING,	//実行中
	SUCCESS,	//成功
	FAILED,		//失敗
	STOP,
};

//改造版
//ノードの種類を派生クラスで定義したもの
//これは基底クラスとなる
class BehaviorNodeBase
{
public:
	BehaviorNodeBase(){}
	virtual ~BehaviorNodeBase(){}

	virtual void Init() = 0;

	virtual bool Judge() = 0;

	virtual NODE_STATUS Run() = 0;

};

