#pragma once
#include "RVector.h"

#include <string>
#include <list>
#include <array>
#include <vector>
#include <compare>

class NavMesh
{
public:
	NavMesh(RVector3 v1, RVector3 v2, RVector3 v3);
	NavMesh() {}
	~NavMesh(){}

	void InitCost();

	//ナビメッシュ頂点情報
	std::array<RVector3, 3> navmashVertex;
	//ナビメッシュ重心点
	RVector3				center;
	//インデックス番号
	std::array<unsigned short, 3> indices;
	std::string indexNumStr;

	//隣接メッシュ
	std::vector<NavMesh*>	navmeshptr;

	//識別番号
	int navMeshNo = -1;

	//ヒューリスティックコスト（エージェントが計算する）
	int heuristicCost = -1;

	//トータルコスト
	int totalCost = -1;

	bool operator > (const NavMesh& str) const
	{
		return (totalCost > str.totalCost);
	}
	bool operator < (const NavMesh& str) const
	{
		return (totalCost < str.totalCost);
	}
	bool operator==(const NavMesh& other) const
	{
		return other.indexNumStr == this->indexNumStr;
	}
	bool operator==(const NavMesh* om) const{
		return om->indexNumStr == this->indexNumStr;
	}
	const bool operator==(NavMesh& other) { return this->indexNumStr == other.indexNumStr; }

	auto operator<=>(const NavMesh &n) const = default;
	
};

//bool NavMesh::operator== (const NavMesh& m1, const NavMesh& m2) {
//	return m1.navMeshNo == m2.navMeshNo;
//}