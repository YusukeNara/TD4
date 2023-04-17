#pragma once

#include <string>

//アクション基底クラス
class BehaviorActionBase
{
public:
	BehaviorActionBase() = default;
	virtual ~BehaviorActionBase() = default;

	//実行クラスの状態
	enum ACTION_STATE
	{
		STANDBY,
		RUNNING,	//実行中
		SUCCESS,	//成功
		FAILED,		//失敗
	};

	ACTION_STATE actionState = STANDBY;

	//実行関数
	virtual ACTION_STATE Run() = 0;

	virtual void Init() = 0;

	std::string actScriptName;
};