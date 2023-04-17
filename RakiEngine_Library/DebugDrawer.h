#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>

//ライブラリコンパイル
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

//使用する自作エンジンの機能のインクルード
#include "NY_Object3D.h"
#include "RVector.h"
#include "RenderTargetManager.h"


class DebugDrawer
{
public:

	static DebugDrawer *GetInstance();

	void Init();

	void Rendering();

	void DrawLine(RVector3 pos1, RVector3 pos2);

private:

	//パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;

	//シェーダーオブジェクト（標準）
	ComPtr<ID3DBlob> line_VS;
	ComPtr<ID3DBlob> line_PS;

	//線頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;



};

