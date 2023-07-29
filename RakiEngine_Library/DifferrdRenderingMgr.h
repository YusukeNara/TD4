#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <array>

#include "DirectionalLight.h"
#include "RTex.h"

//ライブラリコンパイル
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct cbuffer_b0
{
	DirectX::XMFLOAT3 eyePos;	//カメラ座標
	float pad;
};

struct cbuff_bool
{
	bool myBool;
	bool padding[3];
};

struct cbuffer_b1
{
	DirectionalLight::ConstBuffData lightData[4];
};

class DiferredRenderingMgr
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//コンストラクタ、デストラクタ
	DiferredRenderingMgr(){};
	~DiferredRenderingMgr(){};

	//初期化
	void Init(ID3D12Device* dev, ID3D12GraphicsCommandList* cmd);

	//描画
	void Rendering(RTex* gBuffer, RTex* shadowMap);

	//デバッグ描画
	void ShowImGui();



private:
	struct VertexPosUv {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	void ShaderCompile();

	void CreateGraphicsPipeline();

	void UpdateConstBuff();

	//デバイス、コマンドリストのポインタ
	ID3D12Device				*m_dev;
	ID3D12GraphicsCommandList	*m_cmd;
	//グラフィックスパイプライン、ルートシグネチャ
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	//シェーダーオブジェクト
	ComPtr<ID3D10Blob>			m_litVS;
	ComPtr<ID3D10Blob>			m_litPS;
	//ペラポリゴンリソース
	ComPtr<ID3D12Resource>		m_vertBuff;		//頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW	m_vbview;		//VBV
	//定数バッファ（カメラ視点座標）
	ComPtr<ID3D12Resource>		m_constBuffEyePos;
	ComPtr<ID3D12Resource>		m_constBuffDirLight;

	//ライト配列
	std::array<DirectionalLight, 4> directionalLights;

};

