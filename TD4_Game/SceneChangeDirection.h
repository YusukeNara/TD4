#pragma once


class SceneChangeDirection
{
	SceneChangeDirection();

	~SceneChangeDirection();

	//演出初期化
	void Init();
	//シーン開始演出開始
	void PlayInDirection();
	//シーン終了演出開始
	void PlayOutDirection();
	//更新（毎フレーム実行）
	void Update();
	//描画
	void Draw();

private:
	//演出に必要な変数はここ




};

