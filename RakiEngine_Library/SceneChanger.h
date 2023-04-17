#pragma once

//シーン切り替え用インターフェイスクラス
class SceneChanger
{
	virtual ~SceneChanger() = 0;
	virtual void ChangeScene(int nextScene) = 0;//指定シーンに変更する
};

