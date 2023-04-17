#include "Raki_DX12B.h"
#include "Raki_imguiMgr.h"
#include "Raki_DX12B.h"
#include "Raki_WinAPI.h"
#include "RenderTargetManager.h"

bool ImguiMgr::InitImgui(ID3D12Device *dev, HWND hwnd)
{
	//imgui用ヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	HRESULT result;

	result = dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&dsvHeapForImgui));

	//生成失敗
	if (result != S_OK) {
		return false;
	}

	//imguiの初期化
	if (ImGui::CreateContext() == nullptr) {
		bool imgui_isNotCreateContext = false;
		assert(!imgui_isNotCreateContext);
	}

	//windows用初期化
	bool bin_result = ImGui_ImplWin32_Init(Raki_WinAPI::GetHWND());
	if (!bin_result) {
		return false;
	}

	//DirectX12用初期化
	bin_result = ImGui_ImplDX12_Init(
		Raki_DX12B::Get()->GetDevice(),
		3,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		dsvHeapForImgui.Get(),
		dsvHeapForImgui.Get()->GetCPUDescriptorHandleForHeapStart(),
		dsvHeapForImgui.Get()->GetGPUDescriptorHandleForHeapStart()
	);
	if (!bin_result) {
		return false;
	}

	//成功
	return true;
}

void ImguiMgr::NewFrame()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImguiMgr::StartDrawImgui(const char *windowTitle, float posX, float posY)
{
	//ウィンドウ表示
	ImGui::Begin(windowTitle);
	ImGui::SetWindowSize(ImVec2(posX, posY), ImGuiCond_::ImGuiCond_FirstUseEver);
}

void ImguiMgr::EndDrawImgui()
{
	//表示終了、描画
	ImGui::End();
}

void ImguiMgr::SendImguiDrawCommand()
{
	//RenderTargetManagerにバックバッファへの描画を伝える
	RenderTargetManager::GetInstance()->SetDrawBackBuffer();

	ImGui::Render();

	//デスクリプタヒープをセット
	Raki_DX12B::Get()->GetGCommandList()->SetDescriptorHeaps(
		1, dsvHeapForImgui.GetAddressOf()
	);

	//描画実行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), Raki_DX12B::Get()->GetGCommandList());
}

void ImguiMgr::FinalizeImgui()
{
	//終了処理を実行
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool myImgui::InitializeImGui(ID3D12Device *dev, HWND hwnd)
{
	bool result = ImguiMgr::Get()->InitImgui(dev, hwnd);
	return result;
}

void myImgui::StartDrawImGui(const char *windowTitle, float sizeX, float sizeY)
{
	ImguiMgr::Get()->StartDrawImgui(windowTitle, sizeX, sizeY);
}

void myImgui::EndDrawImGui()
{
	ImguiMgr::Get()->EndDrawImgui();
}

void myImgui::FinalizeImGui()
{
	ImguiMgr::Get()->FinalizeImgui();
}
