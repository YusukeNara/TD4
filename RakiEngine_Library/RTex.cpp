#include "RTex.h"
#include "Raki_DX12B.h"

void RenderTextureData::Init(int texwidth, int texheight,float *clearColor,int addBufferNums, RenderTextureOption option[])
{
	//各種リソース生成
	CreateTextureBuffer(texwidth, texheight, clearColor, addBufferNums, option);
	CreateSRVDescriptorHeap(addBufferNums, option);
	CreateRTVDescriptorHeap(addBufferNums);
	CreateDepthBuffer(texwidth, texheight);
	CreateDSVDescriptorHeap(addBufferNums);
}

void RenderTextureData::CreateTextureBuffer(int texture_width, int texture_height,float *clearColor, int addBufferNums, RenderTextureOption option[])
{
	auto hp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	auto resdesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		static_cast<UINT>(texture_width),
		static_cast<UINT>(texture_height),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);
	auto clearvalue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);

	//バッファ生成数分ループ
	for (int i = 0; i < addBufferNums; i++) {

		resdesc = CD3DX12_RESOURCE_DESC::Tex2D(
			option[i].format,
			static_cast<UINT>(texture_width),
			static_cast<UINT>(texture_height),
			1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		);
		clearvalue = CD3DX12_CLEAR_VALUE(option[i].format, option[i].clearColor);

		rtexBuff.push_back(nullptr);
		HRESULT RenderTexture_Create_Result =
			RAKI_DX12B_DEV->CreateCommittedResource(
				&hp,
				D3D12_HEAP_FLAG_NONE,
				&resdesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearvalue,
				IID_PPV_ARGS(&rtexBuff[i])
			);

		rtexBuff[i]->SetName(L"rTexBuffers");

		//生成失敗時は終了
		assert(SUCCEEDED(RenderTexture_Create_Result));
	}

	
}

void RenderTextureData::CreateSRVDescriptorHeap(int bufferCount, RenderTextureOption option[])
{
	HRESULT result;

	//テクスチャ用設定
	D3D12_DESCRIPTOR_HEAP_DESC texdhd = {};
	texdhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	texdhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//バッファ生成数分デスクリプタは生成
	texdhd.NumDescriptors = bufferCount;

	//テクスチャ用デスクリプタヒープ
	result = RAKI_DX12B_DEV->CreateDescriptorHeap(&texdhd, IID_PPV_ARGS(&srvHeap));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < bufferCount; i++) {

		srvDesc.Format = option[i].format;


		RAKI_DX12B_DEV->CreateShaderResourceView(rtexBuff[i].Get(),
			&srvDesc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(srvHeap.Get()->GetCPUDescriptorHandleForHeapStart(), i,
				RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
		);
	}

	srvHeap->SetName(L"RTex_SRV_HEAP");
}

void RenderTextureData::CreateRTVDescriptorHeap(int bufferCount)
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvdhd = {};
	rtvdhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//バッファ生成数分デスクリプタ生成
	rtvdhd.NumDescriptors = bufferCount;

	HRESULT result = RAKI_DX12B_DEV->CreateDescriptorHeap(
		&rtvdhd, IID_PPV_ARGS(&rtvHeap)
	);

	for (int i = 0; i < bufferCount; i++) {
		RAKI_DX12B_DEV->CreateRenderTargetView(rtexBuff[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
				i, RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}

	rtvHeap->SetName(L"RTex_RTV_HEAP");
}

void RenderTextureData::CreateDepthBuffer(int texture_width, int texture_height)
{
	//画像サイズに合わせてバッファを生成する
	CD3DX12_RESOURCE_DESC depthresdesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			texture_width,
			texture_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto clearvalue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	//深度バッファ生成

	HRESULT rtex_depthbuff_create_result = RAKI_DX12B_DEV->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&depthresdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearvalue,
		IID_PPV_ARGS(&depthBuff)
	);

	//生成失敗は強制終了
	assert(SUCCEEDED(rtex_depthbuff_create_result));

	depthBuff->SetName(L"RTex_DepthBuffer");
}

void RenderTextureData::CreateDSVDescriptorHeap(int bufferCount)
{
	D3D12_DESCRIPTOR_HEAP_DESC depthheapdesc{};
	depthheapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	depthheapdesc.NumDescriptors = 1;

	auto rtex_dsv_descheap_create_result = RAKI_DX12B_DEV->CreateDescriptorHeap(
		&depthheapdesc,
		IID_PPV_ARGS(&dsvHeap)
	);
	assert(SUCCEEDED(rtex_dsv_descheap_create_result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {  };
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	RAKI_DX12B_DEV->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

	dsvHeap->SetName(L"RTex_DSV_HEAP");
}

RTex::RTex()
{
	RenderTextureData* tmp = new RenderTextureData;
	rtdata.reset(tmp);
}

RTex::~RTex()
{

}

void RTex::CreateRTex(int texture_width, int texture_height, float* clearColor, int bufferCount, RenderTextureOption* option)
{
	//レンダーテクスチャデータ初期化
	rtdata->Init(texture_width, texture_height, clearColor, bufferCount, option);

	//ビューポート、シザー矩形初期化
	InitViewAndRect(texture_width, texture_height);

	for (int i = 0; i < 4; i++) {
		clearColors[i] = clearColor[i];
	}

	graph_size.first = texture_width;
	graph_size.second = texture_height;
}

void RTex::ClearRenderTarget()
{


	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtdata->rtvHeap->GetCPUDescriptorHandleForHeapStart());
	RAKI_DX12B_CMD->ClearRenderTargetView(rtvh, clearColors.data(), 0, nullptr);

	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtdata->dsvHeap->GetCPUDescriptorHandleForHeapStart());
	RAKI_DX12B_CMD->ClearDepthStencilView(dsvh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void RTex::InitViewAndRect(int width, int height)
{
	//ビューポートはウィンドウサイズに合わせる
	auto temp = CD3DX12_VIEWPORT(0.0f, 0.0f, FLOAT(Raki_WinAPI::window_width), FLOAT(Raki_WinAPI::window_height));
	viewport = temp;

	//シザー矩形は画面左上
	rect = CD3DX12_RECT(0, 0, width, height);

}
