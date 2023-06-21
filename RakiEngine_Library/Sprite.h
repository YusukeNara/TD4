#pragma once
#include <memory>

#include "SpriteManager.h"
#include "NY_Camera.h"
#include "TexManager.h"

class Sprite
{
public:
	//スプライト一枚の情報
	std::unique_ptr<SpriteData> spdata;
	std::unique_ptr<SpriteData> lineSpdata;

	//uvオフセットハンドル（これを増減させることで分割したuv値を適用可能）
	int uvOffsetHandle = 0;

public:
	//スプライト生成（実体生成時に起動でいい？）(デバイス、スプライトサイズ、リソース番号、アンカーポイント、スプライトマネージャーポインタ)
	Sprite(XMFLOAT2 size,UINT resourceID, bool adjustResourceFlag = false,XMFLOAT2 anchor = { 0.0f,0.0f })
	{
		//スプライト生成
		CreateSprite(size, anchor, resourceID, adjustResourceFlag, nullptr);
	};
	//引数なし（別で初期化）
	Sprite();
	~Sprite();

	static void SetSpriteColorParam(float r, float g, float b, float a);

	/// <summary>
	/// 描画のもとになるスプライトデータ生成
	/// </summary>
	/// <param name="resourceID">テクスチャハンドル</param>
	/// <param name="sizeX">描画サイズ</param>
	/// <param name="sizeY">描画サイズ</param>
	/// <param name="reserveDrawCount">描画する数</param>
	void Create(UINT resourceID);

	void CreateRtexSprite(int handle);

	/// <summary>
	/// スプライトを生成し、uv値分割を適用
	/// </summary>
	/// <param name="divAllnum">総分割数</param>
	/// <param name="divX">x方向分割数</param>
	/// <param name="divY">y方向分割数</param>
	/// <param name="sizeX">分割サイズ</param>
	/// <param name="sizeY">分割サイズ</param>
	/// <param name="resourceID">テクスチャハンドル</param>
	void CreateAndSetDivisionUVOffsets(int divAllnum, int divX, int divY, int sizeX, int sizeY,UINT resourceID);

	//頂点インスタンスデータを更新
	void InstanceUpdate();

	//スプライト描画
	void Draw();
	void DrawRenderTexture(int handle);

	void DrawSprite(float posX, float posY, DirectX::XMFLOAT4 color = { 1.f,1.f,1.f,1.f });

	void DrawExtendSprite(float x1, float y1, float x2, float y2, DirectX::XMFLOAT4 color = { 1.f,1.f,1.f,1.f });

	void DrawRotaSprite(float x1, float y1, float x2, float y2, float angle, DirectX::XMFLOAT4 color = { 1.f,1.f,1.f,1.f });

	void DrawLine(float x1, float y1, float x2, float y2, DirectX::XMFLOAT4 color = { 1.f,1.f,1.f,1.f });

	void DrawRTexSprite(int handle, float x1, float y1, float x2, float y2, float angle, DirectX::XMFLOAT4 freedata01 = { 1,1,1,1 });

	void DrawNumSprite(float startX, float startY, float sizeX, float sizeY, int value, DirectX::XMFLOAT4 color = { 1.f,1.f,1.f,1.f });

	void DrawNumSpriteZeroFill(float startX, float startY, float sizeX, float sizeY, int value, int maxDigit, DirectX::XMFLOAT4 color = { 1.f,1.f,1.f,1.f });
	//テクスチャのもとのサイズ
	XMFLOAT2 TEXTURE_DEFAULT_SIZE;

private:
	static DirectX::XMFLOAT4 sprite_color;

	DirectX::XMFLOAT4 sp_color = { 1.0f,1.0f,1.0f,1.0f };

	UINT sizeInsVB;

	UINT instanceDrawCount;

	//生成炭？
	bool isCreated = false;

	static float depthZ;

	//頂点バッファの再確保が必要か？
	bool isVertexBufferNeedResize();

	//頂点バッファのサイズ変更（インスタンシング用バッファ）
	void ResizeVertexInstanceBuffer(UINT newWidthSize);

	//スプライト更新（エンジンで勝手にやる）
	void UpdateSprite();

	bool IsCreated();

	//スプライト初期化(デバイス、スプライトサイズ、アンカーポイント、使用するリソース番号、リソース調整フラグ)
	void CreateSprite(XMFLOAT2 size, XMFLOAT2 anchor, UINT resourceID, bool adjustResourceFlag, uvAnimData* animData = nullptr);

};

///新、スプライト生成の仕方
///
/// テクスチャ番号を指定して作成
/// ↓
/// Updateはしない
/// ↓
/// Draw()以外の描画関数を呼ぶ
/// ↓
/// これ以上描画しないタイミングでDraw()を実行
/// 
/// 描画関数
/// DrawSprite(x,y); 左上基準、通常描画
/// 


