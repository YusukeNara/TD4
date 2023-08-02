#include "fbxModel.h"

#include <iostream>

const int fbxModel::BONE_INDICES_MAX;

fbxModel::fbxModel()
{
	ExportEngineLogText(L"FBXmodel", L"constructor", L"new fbx model", 0);
}

fbxModel::~fbxModel()
{
	ExportEngineLogText(L"FBXmodel", L"destructor", L"delete fbx model", 0);
	if (fbxScene != nullptr) { 
		TexManager::DeleteTexture(material.texNumber);
		fbxScene->Destroy(); 
	}

}

void fbxModel::CreateBuffers()
{
	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(fbxVertex) * vertices.size());

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resdesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	result = RAKI_DX12B_DEV->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertbuff)
	); struct SkinAnimationPlayInfo
	{
		float start;
		float end;
		std::string name;
	};
	if (result != S_OK) {
		std::cout << std::system_category().message(result) << std::endl;
	}

	fbxVertex* vertmap = nullptr;
	result = vertbuff->Map(0, nullptr, (void**)&vertmap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertmap);
		vertbuff->Unmap(0, nullptr);
	}

	vbview.BufferLocation = vertbuff->GetGPUVirtualAddress();
	vbview.SizeInBytes = sizeVB;
	vbview.StrideInBytes = sizeof(vertices[0]);

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	auto resdescIB = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);

	result = RAKI_DX12B_DEV->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdescIB,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexbuff)
	);

	unsigned short* indexmap = nullptr;
	result = indexbuff->Map(0, nullptr, (void**)&indexmap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexmap);
		indexbuff->Unmap(0, nullptr);
	}

	ibview.BufferLocation = indexbuff->GetGPUVirtualAddress();
	ibview.SizeInBytes = sizeIB;
	ibview.Format = DXGI_FORMAT_R16_UINT;

}

void fbxModel::Draw()
{
	RAKI_DX12B_CMD->IASetVertexBuffers(0, 1, &vbview);

	RAKI_DX12B_CMD->IASetIndexBuffer(&ibview);

	ID3D12DescriptorHeap* ppheaps[] = { TexManager::texDsvHeap.Get() };
	RAKI_DX12B_CMD->SetDescriptorHeaps(_countof(ppheaps), ppheaps);

	RAKI_DX12B_CMD->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(),
			material.texNumber, RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	RAKI_DX12B_CMD->DrawIndexedInstanced(UINT(indices.size()), 1, 0, 0, 0);

}
