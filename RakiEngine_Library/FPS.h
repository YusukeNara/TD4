#pragma once
#include <windows.h>
#include <tchar.h>
#include <sstream>

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

/// <summary>
/// FPS管理用クラス、複数ある意味がないのでシングルトンパターン採用
/// </summary>
class FPS final
{
private:
	const float MIN_FREAM_TIME = 1.0f / 60.0f;
	float frameTime = 0;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	// fpsを取得するなら0で初期化しないとゴミが混ざってマイナスから始まったりする(かも知れない)
	float fps = 0;

	//コンストラクタ、デストラクタを隠蔽
	FPS() {};
	~FPS() {};

public:

	//初回実行
	void Start();

	//FPS処理の実行
	void run();

	//インスタンス取得
	static FPS *Get();

	//コピーコンストラクタ、代入演算子無効化
	FPS(const FPS &obj) = delete;
	FPS &operator=(const FPS &obj) = delete;

	int fCount = 0;

};

