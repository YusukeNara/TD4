#pragma once

#pragma warning (push)
#pragma warning (disable:6001)
#pragma warning (disable:26812)
#pragma warning (disable:26451)
#pragma warning (disable:26495)

#include "Raki_DX12B.h"
#include "TexManager.h"

#include <Windows.h>
#include <wrl.h>
#include <fbxsdk.h>

#include <string>
#include <vector>
#include <memory>

class Node
{
public:
	std::string name;

	DirectX::XMVECTOR scaling;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR translation;
	DirectX::XMMATRIX transform;
	DirectX::XMMATRIX globalTransform;

	Node* parent = nullptr;
};

//頂点データ
struct fbxVertex
{
	DirectX::XMFLOAT3 pos;		//xyz
	DirectX::XMFLOAT3 normal;	//法線
	DirectX::XMFLOAT2 uv;		//uv
	UINT boneIndex[4];
	float boneWeight[4];
	int controlPointIndex;		//コントロールポイントのインデックス番号
};

struct Bone
{
	std::string name;
	DirectX::XMMATRIX invInitialBone;
	FbxCluster* fbxCluster;
	Bone(std::string name) {
		this->name = name;
	}

};

//マテリアルデータ構造体
struct fbxMaterial
{
	std::string   mtlName;      //マテリアル名(objファイルからではない場合、使わなくていい？)
	DirectX::XMFLOAT3 ambient;      //アンビエント
	DirectX::XMFLOAT3 diffuse;      //ディフューズ
	DirectX::XMFLOAT3 specurar;     //スペキュラー
	float    alpha;        //アルファ
	std::string		texFileName;  //テクスチャ名
	UINT			texNumber;    //テクスチャ番号(使用する場合必ず必要)
	DirectX::TexMetadata mdata = {};
	DirectX::ScratchImage simg = {};

	//コンストラクタ
	fbxMaterial() {
		ambient = { 1.0f,1.0f,1.0f };
		diffuse = { 0.8f,0.8f,0.8f };
		specurar = { 0.8f,0.8f,0.8f };
		alpha = 1.0f;
	}
};

struct SkinAnimationPlayInfo
{
	float start;
	float end;
	std::string name;
};

class fbxModel
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using string = std::string;
	template <class T> using vector = std::vector<T>;



public:
	friend class FbxLoader;
	fbxModel();
	~fbxModel();

	void CreateBuffers();

	void Draw();

	inline const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	inline const fbxMaterial& GetMaterial() { return material; }

	inline std::vector<Bone>& GetBones() { return bones; }

	FbxScene* GetFbxScene() { return fbxScene; }

	static const int BONE_INDICES_MAX = 4;

private:
	//モデル名
	std::string name;

	std::vector<Node> nodes;
	std::vector<Bone> bones;

	Node* meshNode = nullptr;

	std::vector<fbxVertex> vertices;
	ComPtr<ID3D12Resource> vertbuff;

	std::vector<unsigned short> indices;
	ComPtr<ID3D12Resource>		indexbuff;

	D3D12_VERTEX_BUFFER_VIEW vbview = {};
	D3D12_INDEX_BUFFER_VIEW ibview = {};

	int ctrlPointIndicesCount;

	fbxMaterial material;

	FbxScene *fbxScene = nullptr;

	std::vector<SkinAnimationPlayInfo> animinfo;
};

#pragma warning (pop)