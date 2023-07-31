#pragma once

#include <d3dx12.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <array>

#pragma comment(lib, "d3d12.lib")

//uvオフセット格納コンテナ
struct UVOFFSETS {
	DirectX::XMFLOAT2 offsetLT;	//左上
	DirectX::XMFLOAT2 offsetRT;	//右上
	DirectX::XMFLOAT2 offsetLB;	//左下
	DirectX::XMFLOAT2 offsetRB;	//右下
};

//uvアニメーション用データ
class uvAnimData {
private:
	//uvオフセット格納コンテナ
	std::vector<UVOFFSETS> uvOffsets;

public:
	//コンスタラクタ、デストラクタ
	uvAnimData() {
		usingNo = 0;
	};
	~uvAnimData() {};

	//使用中のアニメーション番号
	int usingNo;

	//オフセット作成
	void AddOffsets(UVOFFSETS offset);
	//オフセット取得
	UVOFFSETS GetOffset();

	int Getsize();

};

//テクスチャ一枚のデータ
struct texture {
	UINT									texNumber = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource>	texBuff = nullptr;	//テクスチャバッファ
	const DirectX::Image* img = nullptr;
	DirectX::TexMetadata					metaData;
	DirectX::ScratchImage					scratchImg;
	//アニメーション用uv管理
	std::vector<DirectX::XMFLOAT2>			uv_offsets;	//uv値オフセット配列
};

//マルチパスレンダリングをするときにあると今後が楽だと思って作った
//けど
class TexManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	TexManager() {};
	~TexManager();

	static ID3D12Device *dev;

public:

	//テクスチャデータ配列
	static std::array<texture*,2048> textureData;
	//デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> texDsvHeap;
	//テクスチャ最大数
	static const int MAX_TEXNUM;


public:
	/// <summary>
	/// TexManagerの初期化、ディスクリプタヒープ生成
	/// </summary>
	static void InitTexManager();

	/// <summary>
	/// テクスチャのロード
	/// </summary>
	/// <param name="filename">const char型で画像ファイルの名前</param>
	/// <returns>格納したバッファの番号</returns>
	static UINT LoadTexture(const char *filename);

	static UINT LoadTexture(std::string filename);

	static texture GetTextureState(UINT bufferRocate);

	/// <summary>
	/// テクスチャの分割読み込み
	/// </summary>
	/// <param name="filename">画像ファイルの名前</param>
	/// <param name="numDivTex">分割数（右方向）</param>
	/// <param name="sizeX">1枚のサイズ</param>
	/// <returns>格納したテクスチャの場所を配列で</returns>
	static UINT LoadDivTextureTest(uvAnimData *data, const char *filename, const int numDivTex, int sizeX);

	static void DeleteTexture(UINT texhandle);
};

