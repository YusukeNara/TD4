#pragma once
#include "NavMash.h"
#include "NavMeshData.h"

class NavMeshAstar
{
public:

	//探索するナビメッシュデータを設定
	void SetNavMeshData(std::vector<NavMesh> &navMeshes);

	void NavMeshSearchAstar(NavMesh *start, NavMesh *goal, std::vector<RVector3> &result);

	void NavMeshSearchAster(RVector3 nowPos, RVector3 targetPos, std::vector<RVector3>& result);

	bool CheckNavMeshAddOpenAdd(std::vector<NavMesh*> &open, std::vector<NavMesh*> &close, NavMesh* checkMesh, int cost);

	int CaclHeuristicCost(NavMesh* goalMesh, NavMesh* targetMesh);

	//与えられたルートと移動させたい座標をもとに次の方向を示す
	RVector3 MoveWaypointDirectLine(float permissible, RVector3& nowpos, bool &isMoved);
	RVector3 MoveWaypointDirectLine(std::vector<NavMesh> &root, RVector3& nowpos);
	RVector3 MoveWaypointDirectLine(std::vector<RVector3>& root, RVector3& nowpos, float permissible, int& index, bool& usMoved);
	//スプライン曲線での移動


	//座標から近いメッシュを返す
	NavMesh* NowStandingMesh(RVector3 targetPos);

	//ナビメッシュデータ保存先
	std::vector<NavMesh*> navMeshData;

	//経路情報保存先
	std::vector<NavMesh*> rootData;

};

