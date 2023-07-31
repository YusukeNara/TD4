#pragma once
//DirectX
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
//c++
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
//myEngine
#include "NY_Model.h"
#include "fbxModel.h"
#include "RVector.h"
#include "RTex.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

//定数バッファデータ構造体B0（座標系なので、同じオブジェクトを複数出すなら増やさないとだめ）
struct ConstBufferDataB0
{
	XMMATRIX mat;
	XMMATRIX wMat;
	XMFLOAT4 color;
	XMMATRIX lightCamMat;
};

//定数バッファデータ構造体B1（マテリアルを使用->モデル依存なので複数いらない）
struct ConstBufferDataB1
{
	XMFLOAT3 amdient;  //アンビエント
	float pad1;//パディング
	XMFLOAT3 diffuse;  //ディフューズ
	float pad2;//パディング
	XMFLOAT3 specular; //スペキュラー
	float alpha;       //アルファ
};

struct ConstBufferDataSkin
{
	XMMATRIX bones[64];
};

//ビルボードパーティクル頂点データ
struct ParticleVertex
{
	XMFLOAT3 pos;
};

enum class ANIMATION_PLAYMODE
{
	ANIM_MODE_FIRST,
	ANIM_MODE_ROOP,
};



// 2022/03/16 クラスの依存関係を変更
//mgr -> object3d -> model3d
//このクラスは中間に位置させ、modelを所有する
//ただし作成時の処理を変更、mgrにモデルのロード関数を置く
//内部の処理は、model3dを作成 -> モデルを読み込み -> object3dに所有させる


class Object3d
{
public:
	//オブジェクトの名前
	string tagName;

	//色情報
	XMFLOAT4 color = { 1,1,1,1 };

	//ビルボードフラグ
	bool isBillBoard;

	//影描画フラグ
	bool isDrawShadow = false;

public:
	//コンストラクタ
	Object3d() {
		isBillBoard = false;
		isDirty = true;
		//モデルデータ（空）を作成
		model = make_shared<Model3D>();
		fbxmodel = make_shared<fbxModel>();
	};
	~Object3d() {
		
	}

	//オブジェクトの初期化
	//static void InitObject3D(Object3d *obj, ID3D12Device *dev);
	void InitObject3D(ID3D12Device *dev);

	//ロード済モデルデータの設定
	void SetLoadedModelData(Model3D *loadedModel);

	void SetLoadedModelData(std::shared_ptr<Model3D> loadedmodel);
	void SetLoadedModelData(std::shared_ptr<fbxModel> loadedmodel);

	//変換行列の設定
	void SetWorldMatrix(XMMATRIX matWorld);
	//アフィン変換情報設定
	void SetAffineParam(RVector3 scale, RVector3 rot, RVector3 trans);
	void SetAffineParamScale(RVector3 scale);
	void SetAffineParamRotate(RVector3 rot);
	void SetAffineParamTranslate(RVector3 trans);

	//オブジェクト更新
	void UpdateObject3D();
	//ビルボード更新（カメラオブジェクトをそのまま取り込んで、ビルボード用の更新処理を行う）
	void UpdateBillBoard3D();

	void DrawShadow();

	//オブジェクト描画
	void DrawObject();

	//レンダーテクスチャを使用した描画
	void DrawRTexObject(int rtHandle);
	void DrawRTexObject(RTex* rt);

	//モデルデータをロード
	void LoadAndSetModelData(string modelname);
	void LoadAndSetModelData_Fbx(string filename);
	//モデルデータを別オブジェクトから設定する
	void SetAnotherObjectModelData(Object3d *anotherObj);

	void CreateModel_Tile(float x_size, float y_size, float x_uv, float y_uv, UINT useTexNum);

	void CreateModel_Box(float size, float uv_x, float uv_y, UINT useTexNum);

	//アニメーション関連

	//再生
	void PlayAnimation(ANIMATION_PLAYMODE playmode, int animNum = 0);

	void ResetAnimation();

	void PauseAnimation();

	void StopAnimation();

	//デバッグ表示
	void DisplayObjectStatus(bool isDisplay);

	//アフィン変換情報
	RVector3 scale = { 1,1,1 };
	RVector3 rotation = { 0,0,0 };
	RVector3 position = { 0,0,0 };

private:
	// モデルデータ（ほかのオブジェクトでモデルデータは同一のものを使う場合に備えて、shared_ptrを使用）
	shared_ptr<Model3D> model;
	shared_ptr<fbxModel> fbxmodel;


	//ワールド変換行列
	XMMATRIX matWorld;

	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;
	ComPtr<ID3D12Resource> constBuffSkin;

	//定数バッファビューハンドル(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV;
	//定数バッファビューハンドル(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;

	//アニメーション管理系
	FbxTime frameTime;
	FbxTime startTime;
	FbxTime endTime;
	FbxTime currentTime;
	ANIMATION_PLAYMODE playmode;

	bool isPlay = false;


	//ダーティフラグ
	bool isDirty;

	//影描画フラグ

	//適用するリソースの番号
	UINT resourceNumber;

	//マルチパスレンダリングを使用した描画（マルチパスで作ったリソースを使って描画）
	void DrawMultiPassResource();

	enum isWhichModel {
		MODEL_DATA_OBJ,
		MODEL_DATA_FBX,
	};

	//どのモデルデータか？
	isWhichModel isThisModel;

	//現在再生中のアニメーション番号
	int playAnimNum = 0;

	bool isDisplayAffineParam = true;
	bool isDisplayMaterial = true;
	bool isDisplayBone = true;
};





