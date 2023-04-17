#pragma once

// 07:19 12:00時点 このクラスとその派生クラスは未使用です

//光源のプログラムは本当に継承で正しいのか確認すること！
class BaseLight
{
	//光源初期化
	virtual void Init() = 0;
	//光源データアップロード
	virtual void Upload() = 0;
	

};

