#pragma once

//これをインクルードするだけで、3d描画関連を使用可能にする

//c++関係
#include <iostream>
#include <vector>
#include <memory>

//DirectX機能関係
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>

// 名前空間（良くないから後で消す）
using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

//ライブラリコンパイル
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

//使用する自作エンジンの機能のインクルード
#include "NY_Object3D.h"
#include "RVector.h"
#include "RenderTargetManager.h"

//3Dパイプライン
typedef struct Pipeline3D
{
	//パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;

}PipelineSet;

/// 12/14
///データの管理をマネージャーが一括で行うようにするために
///NY_Object3dを構造体化し、マネージャーがコンテナで管理できるようにする
/// 
/// テクスチャデータをtexManagerで管理するように変更
/// 
/// 2022/3/16
/// クラスの関係性を変更
/// mgr -> object3d -> model3d
/// このクラスは3d関係の上側に位置し、object3dと、簡易描画クラスを管理する


/// <summary>
/// 一つだけ使えればいいのでシングルトンパターン
/// </summary>
class NY_Object3DManager
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	//----------パイプラインステートオブジェクト----------//

	//モデル描画用パイプラインステート
	Pipeline3D object3dPipelineSet;
	//即時描画用パイプライン
	Pipeline3D quickDrawPipelineSet;
	//マルチパステクスチャ用
	Pipeline3D mpTexPipelineSet;

	//----------パイプラインステート構造体--------------//

	//パイプラインステート構造体を複数作って、適宜割り当てる形でもいいのか？

	//モデル用グラフィックスパイプラインステート構造体
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineStateDesc;

	//----------シェーダーオブジェクト----------//

	//シェーダーオブジェクト（標準）
	ComPtr<ID3DBlob> obj_VS;
	ComPtr<ID3DBlob> obj_GS;
	ComPtr<ID3DBlob> obj_PS;
	ComPtr<ID3DBlob> fbx_VS;
	ComPtr<ID3DBlob> shadow_VS;
	ComPtr<ID3DBlob> shadow_PS;
	ComPtr<ID3DBlob> shadowFBX_VS;

	//シェーダーオブジェクト（即時描画用）
	ComPtr<ID3DBlob> qd_BoxGS;

	//----------object3dコンテナ----------//
	vector<Object3d*> objects;

	//----------その他----------//

	//デフォルトテクスチャの色
	XMFLOAT4 defColor = { 1.0,0.0,0.0,1.0 };

	//--------------------ディファードレンダリング用変数-------------------------------//

	//ディファードレンダリングGBuffer用グラフィックスパイプライン設定
	Pipeline3D m_diferredRenderingPipeline;
	//ディファードレンダリング用ステート構造体
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_diferredRenderingStateDesc;
	//FBX用のパイプラインステートがいるらしい...
	Pipeline3D m_fbxPipeline;

	//さらにシャドウマップレンダリング用パイプライン
	Pipeline3D m_shadowMapPipeline;
	Pipeline3D m_shadowMapFbxPipeline;

	//GBuffer(RTex)
public:
	RTex m_gBuffer;

	RTex m_shadomMap;
private:

	//コンストラクタ、デストラクタ
	NY_Object3DManager() {};
	~NY_Object3DManager();

public:
	//コピーコンストラクタ無効化
	NY_Object3DManager(const NY_Object3DManager &obj) = delete;
	//代入演算子無効化
	NY_Object3DManager &operator=(const NY_Object3DManager &obj) = delete;

	//インスタンス取得
	static NY_Object3DManager *Get() {
		static NY_Object3DManager obj;
		return &obj;
	}
	
	//共通ビューポート行列
	XMMATRIX matViewport{};

	/// <summary>
	/// <para>NY_Object3DManagerの生成</para>
	/// </summary>
	/// <returns>成否</returns>
	bool CreateObject3DManager();

	void FinalizeObject3DManager();

	/// <summary>
	/// object3dの作成、コンテナに格納
	/// </summary>
	Object3d *CreateObject3d(Model3D *modelData);

	Object3d *CreateModel_Tile(float x_size, float y_size, float x_uv, float y_uv, UINT useTexNum);

	Object3d* CreateModel_Box(float size, float uv_x, float uv_y, UINT useTexNum);

	/// <summary>
	/// オブジェクトの削除
	/// </summary>
	/// <param name="obj">消すObject3Dのポインタ</param>
	void DeleteObject3d(Object3d *obj);

	/// <summary>
	/// 生成済オブジェクトの一括更新
	/// </summary>
	void UpdateAllObjects();

	void ClearObjects();

	/// <summary>
	/// 3Dオブジェクトの描画を実行するための前準備を行う
	/// </summary>
	/// <param name="cmd">ID3D12GraphicsCommandListのポインタ</param>
	void SetCommonBeginDrawObject3D();

	void CloseDrawObject3D();

	void SetCommonBeginDrawObject3DFeatRTex(int rtHandle);

	void SetRestartObject3D();

	void SetCommonBeginDrawFBX();

	void SetCommonBeginDrawObject3DFeatRTex(RTex* rt);

	void SetCommonBeginDrawObject3D2MultiPassRenderResource();

	void SetCommonBeginDrawShadow();

	void SetCommonBeginDrawShadow_FBX();

	void ReturnShadowToDifferd();

	void ShadowMapClear();

	static ID3D12Device *GetDev() {
		return dev;
	}

private:

	//そのフレームで更新を行ったか？
	bool isUpdated = false;

	//デバイスのポインタ
	static ID3D12Device *dev;

	/// <summary>
	/// 即時描画用ジオメトリシェーダーのコンパイル（生成関数で実行）
	/// </summary>
	/// <returns>全シェーダーコンパイルの成否</returns>
	HRESULT QuickDrawShadersCompile();

	/// <summary>
	/// クラス内で使用するグラフィックスパイプラインの生成
	/// </summary>
	/// <param name="dev">ID3D12Deviceのポインタ</param>
	/// <returns>作成したPipeline3D構造体</returns>
	Pipeline3D Create3DPipelineState(ID3D12Device *dev);

	Pipeline3D CreateMPPipelineState(Pipeline3D defaultPP);

	Pipeline3D CreateDiferredRenderingPipelineState();

	Pipeline3D CreateFbxPipeline();

	Pipeline3D CreateShadowMapPipeline();

	Pipeline3D CreateShadowMapFbxPipeline();

};

/// ----------- 記述量の削減のためのラップ関数群 ---------- ///

/// <summary>
/// NY_Object3DManagerの初期化
/// </summary>
/// <param name="dev">ID3D12Deviceのポインタ</param>
/// <param name="window_w">ウィンドウ横幅</param>
/// <param name="window_h">ウィンドウ縦幅</param>
inline void InitializeObject3DManager() {

	//NY_Object3DManagerを生成、初期化
	NY_Object3DManager::Get()->CreateObject3DManager();
}

/// <summary>
/// Object3dの生成、管理クラスのコンテナに格納
/// </summary>
/// <param name="modelData">NY_Model3Dのモデルデータ</param>
/// <param name="pos">初期位置を設定する</param>
/// <returns>生成されたObject3dを示すポインタ</returns>
//Object3d *CreateObject3d(Model3D *modelData, RVector3 pos = { 0,0,0 });

/// <summary>
/// Objファイルからのデータ読み込み
/// </summary>
/// <param name="modelname">ロードする.objのファイル名</param>
/// <returns>生成したobject3dのポインタ</returns>
Object3d *LoadModel_ObjFile(string modelname);

Object3d* LoadModel_FBXFile(string modelname);

Object3d* SetModel_FBX(std::shared_ptr<fbxModel> fbxModelData);

/// <summary>
/// Object3dデータの削除
/// </summary>
/// <param name="obj"></param>
inline void DeleteObject3d(Object3d *obj) {

	NY_Object3DManager::Get()->DeleteObject3d(obj);
}

inline void UpdateObject() {
	//全オブジェクトの更新
	NY_Object3DManager::Get()->UpdateAllObjects();
}

//描画関数
void DrawObject3d(Object3d *obj);