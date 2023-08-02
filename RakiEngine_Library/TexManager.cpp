#include "TexManager.h"
#include "Raki_DX12B.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include "RakiUtility.h"


std::array<texture*,2048>						TexManager::textureData;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	TexManager::texDsvHeap;
const int										TexManager::MAX_TEXNUM = 2048;

ID3D12Device *TexManager::dev;

TexManager::~TexManager()
{
    for (int i = 0; i < MAX_TEXNUM; i++) {
        if (textureData[i]) {
            DeleteTexture(i);
        }
    }
}

void TexManager::InitTexManager()
{
    HRESULT result;
    //デスクリプタヒープ生成
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = MAX_TEXNUM;
    result = Raki_DX12B::Get()->GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&texDsvHeap));

    texDsvHeap->SetName(L"TextureManager_SRV_HEAP");

    dev = Raki_DX12B::Get()->GetDevice();
}

UINT TexManager::LoadTexture(const char *filename)
{
    std::string fp = filename;
    //拡張子を取得し、対応するロード方法を選択
    size_t pos1;
    std::wstring exceptExt;
    std::wstring filepath, fileExt_;
    int buffsize = MultiByteToWideChar(CP_ACP, 0, fp.c_str(), -1, nullptr, 0);
    filepath.resize(buffsize);
    MultiByteToWideChar(CP_ACP, 0, fp.c_str(), -1, &filepath[0], buffsize);

    pos1 = filepath.rfind('.');

    if (pos1 != std::wstring::npos) {
        fileExt_ = filepath.substr(pos1 + 1, filepath.size() - pos1 - 1);
        exceptExt = filepath.substr(0, pos1);
    }
    else {
        fileExt_ = L"";
        exceptExt = filepath;
    }

    //ヌルチェック
    assert(filename != nullptr);

    HRESULT result;
    // 使用するテクスチャの番号を設定
    UINT useTexIndexNum = 0;
    for (int i = 0; i < MAX_TEXNUM; i++) {
        //bool isNot = std::ranges::any_of(textureData.begin(),textureData.end(), [i](texture *t) { return t->texNumber == i; });
        //空のテクスチャを発見、番号重複がない
        if (textureData[i] == nullptr)
        {
            //番号設定
            useTexIndexNum = (UINT)i;
            textureData[i] = new texture;
            textureData[i]->texNumber = i;
            break;
        }
    }

    // const char => wchar_t
    size_t newsize = strlen(filename) + 1;
    //wchar_tに変換した文字列を格納する変数を作成
    wchar_t *FileName = new wchar_t[newsize];
    //wchar_tに変換
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, FileName, newsize, filename, _TRUNCATE);

    //ddsはdds、それ以外はWIC

    if (wcscmp(fileExt_.c_str(), L"dds") == 0) {
        // WICテクスチャのロード
        result = LoadFromDDSFile(FileName,
            DDS_FLAGS_NONE,
            &textureData[useTexIndexNum]->metaData,
            textureData[useTexIndexNum]->scratchImg);
        textureData[useTexIndexNum]->img = textureData[useTexIndexNum]->scratchImg.GetImage(0, 0, 0);
    }
    else {
        // WICテクスチャのロード
        result = LoadFromWICFile(FileName,
            WIC_FLAGS_IGNORE_SRGB,
            &textureData[useTexIndexNum]->metaData,
            textureData[useTexIndexNum]->scratchImg);
        textureData[useTexIndexNum]->img = textureData[useTexIndexNum]->scratchImg.GetImage(0, 0, 0);
    }


    if (FAILED(result)) {
        std::wstringstream stream;
        stream << std::system_category().message(result).c_str() << std::endl;
        OutputDebugString(stream.str().c_str());
        return -1;
    }

    // テクスチャバッファ生成
    D3D12_HEAP_PROPERTIES texHeapProp{};//テクスチャヒープ設定
    texHeapProp.Type                    = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty         = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference    = D3D12_MEMORY_POOL_L0;

    D3D12_RESOURCE_DESC texresDesc{};//リソース設定
    texresDesc.Dimension        = static_cast<D3D12_RESOURCE_DIMENSION>(textureData[useTexIndexNum]->metaData.dimension);//2Dテクスチャ用
    texresDesc.Format           = textureData[useTexIndexNum]->metaData.format;//RGBAフォーマット
    texresDesc.Width            = textureData[useTexIndexNum]->metaData.width;//横
    texresDesc.Height           = (UINT)textureData[useTexIndexNum]->metaData.height;//縦
    texresDesc.DepthOrArraySize = (UINT16)textureData[useTexIndexNum]->metaData.arraySize;
    texresDesc.MipLevels        = (UINT16)textureData[useTexIndexNum]->metaData.mipLevels;
    texresDesc.SampleDesc.Count = 1;

    result = RAKI_DX12B_DEV->CreateCommittedResource(//GPUリソース生成
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &texresDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&textureData[useTexIndexNum]->texBuff)
    );

    if (FAILED(result)) {
        std::wstringstream stream;
        stream << std::system_category().message(result).c_str() << std::endl;
        OutputDebugString(stream.str().c_str());
        return -1;
    }

    //テクスチャバッファへのデータ転送
    result = textureData[useTexIndexNum]->texBuff->WriteToSubresource(
        0,
        nullptr,
        textureData[useTexIndexNum]->img->pixels,
        (UINT)textureData[useTexIndexNum]->img->rowPitch,
        (UINT)textureData[useTexIndexNum]->img->slicePitch
    );

    if (FAILED(result)) {
        std::wstringstream stream;
        stream << std::system_category().message(result).c_str() << std::endl;
        OutputDebugString(stream.str().c_str());
        return -1;
    }

    //シェーダーリソースビュー設定
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureData[useTexIndexNum]->metaData.format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    //ヒープにシェーダーリソースビュー作成
    D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = texDsvHeap.Get()->GetCPUDescriptorHandleForHeapStart();
    D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart();
    dev->CreateShaderResourceView(textureData[useTexIndexNum]->texBuff.Get(), &srvDesc,
        CD3DX12_CPU_DESCRIPTOR_HANDLE(texDsvHeap.Get()->GetCPUDescriptorHandleForHeapStart(), useTexIndexNum,
            dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

    std::string texname = filename;
    std::string dataName = "TextureBuffer_" + texname;
    std::wstring nametemp = std::wstring(dataName.begin(), dataName.end());
    textureData[useTexIndexNum]->texBuff.Get()->SetName(nametemp.c_str());



    std::wstring log = L"Loaded Texture TexName : ";
    std::wstring fn = rutility::charTowstring(filename);
    std::wstring space = L" : ";
    std::wstring indexNum = L"texture Array Index:";
    std::wstring inum = std::to_wstring(useTexIndexNum);

    std::wstring outputText = log + fn + space + indexNum + inum;


    ExportEngineLogText(L"TexManager", L"LoadTexture()", outputText.c_str(), 175);

    return useTexIndexNum;
}

UINT TexManager::LoadTexture(std::string filename)
{
    UINT texNumber = LoadTexture(filename.c_str());
    return texNumber;
}

texture TexManager::GetTextureState(UINT bufferRocate)
{

    return texture();
}

UINT TexManager::LoadDivTextureTest(uvAnimData *data,const char *filename, const int numDivTex, int sizeX)
{
    //テクスチャ読み込み
    UINT useNo = LoadTexture(filename);

    //該当テクスチャのuvオフセットを用意

    //アニメーション1枚の画像サイズを算出
    float animTexSizeX = float(textureData[useNo]->metaData.width) / float(numDivTex);
    //1枚のサイズ / 全体のサイズ = 1枚あたりのuvのオフセット値
    float uvoffSetParam = animTexSizeX / float(textureData[useNo]->metaData.width);
    //uvのオフセット値を元にuvオフセットを設定
    for (int i = 0; i < numDivTex; i++) {
        UVOFFSETS offset = {
            {uvoffSetParam * float(i),0 * -1},//左上
            {uvoffSetParam * float(i) + uvoffSetParam,0 * -1},//右上
            {uvoffSetParam * float(i),1 * -1},//左下
            {uvoffSetParam * float(i) + uvoffSetParam,1 * -1},//右下
        };

        data->AddOffsets(offset);
    }

    data->usingNo = 0;

    return useNo;
}

void TexManager::DeleteTexture(UINT texhandle)
{
    //範囲外参照防止
    if (texhandle < 0 || texhandle > 2047) {
        return;
    }

    if (textureData[texhandle] != nullptr) {
        delete textureData[texhandle];
        textureData[texhandle] = nullptr;
        ExportEngineLogText(L"TexManager", L"DeleteTexture()", L"Texture deleted.", int(texhandle));
    }
}

void uvAnimData::AddOffsets(UVOFFSETS offset)
{
    //オフセット格納
    uvOffsets.push_back(offset);
}

UVOFFSETS uvAnimData::GetOffset()
{
    if (usingNo < 0 || usingNo > uvOffsets.size()) {
        return uvOffsets[0];
    }

    return uvOffsets[usingNo];
}

int uvAnimData::Getsize()
{
    return int(uvOffsets.size());
}
