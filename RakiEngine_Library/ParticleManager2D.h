#pragma once
#include "Sprite.h"
#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <vector>
#include <memory>
#include "RVector.h"

//パーティクルパラメーター
struct ParticleGrainState2D {
private:
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:

	//起点
	RVector3 position;
	//速度
	RVector3 vel;
	//速度上限
	RVector3 velMax;
	//速度下限
	RVector3 velMin;
	//加速度
	RVector3 acc;

	//色関連
	//色補完開始点
	XMFLOAT4 color_start;
	//色補完終了点
	XMFLOAT4 color_end;

	//アフィン変換
	//スケーリング
	float scale_start;
	float scale_end;
	//回転
	float rot_start;
	float rot_end;
	//寿命
	int aliveTime;


	//ランダムスポーンフラグ
	bool isRandomSpawn;
	//スポーン範囲
	RVector3 position_spawnRange1;
	RVector3 position_spawnRange2;
};

//パーティクル一粒のクラス
struct Grain2d
{
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public:
	//座標
	RVector3	pos		= { 0,0,0 };
	//速度
	RVector3	vel		= { 0,0,0 };
	//加速度
	RVector3	acc		= { 0,0,0 };
	//色
	XMFLOAT4	color	= { 1,1,1,1 };
	//スケーリング
	float		scale	= 1.0f;
	//サイズ
	XMFLOAT2	drawsize = { 16.0f,16.0f };
	//回転(2dなのでz軸)
	float		rot		= 0.0f;
	//初期値
	XMFLOAT4	s_color		= { 1,1,1,1 };
	float		s_scale		= 1.0f;
	float		s_rotation	= 0.0f;
	// 最終値
	XMFLOAT4	e_color		= { 1,1,1,1 };
	float		e_scale		= 0.0f;
	float		e_rotation	= 0.0f;
	// 現在フレーム
	int nowFrame = 0;
	// 終了フレーム
	int endFrame = 0;
};

//
class ParticlePrototype2D : public Grain2d
{
public:
	ParticlePrototype2D() {};
	~ParticlePrototype2D() {};
	//初期化
	virtual void Init() = 0;
	//更新
	virtual void Update() = 0;
	//クローン生成（起点座標だけ取得する? ユーザ定義でおｋだった）
	virtual ParticlePrototype2D* clone() = 0;

	//描画、終了は必要ない（実際の描画はエミッターでやるので）
};

class ParticleManager2D
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	ParticleManager2D();
	~ParticleManager2D();

	void Initialize(UINT graphHandle);

	void Add(ParticleGrainState2D &pgState2d);

	void Update();

	void Draw();

	void Prototype_Set(ParticlePrototype2D* ins);

	void Prototype_Add();

	void Prototype_Update();

	void Prototype_Draw();

	ParticlePrototype2D* GetPrototypeInstance() { return _prototype.get(); }

private:
	//3Dと異なり、グラフィックパイプラインなどはスプライトのものを使うのでナシ

	//スプライトデータ
	Sprite _psprite;
	//パーティクルプロトタイプポインタ
	std::unique_ptr<ParticlePrototype2D> _prototype;
	//パーティクル情報コンテナ
	std::vector<std::unique_ptr<ParticlePrototype2D>> _pplist;

	//通常パーティクルポインタ
	std::vector<std::unique_ptr<Grain2d>> _grains;

};

