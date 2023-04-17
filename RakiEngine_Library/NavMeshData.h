#pragma once
#include <string>
#include <vector>

#include "NavMash.h"



class NavMeshData
{
public:
	void LoadNavMesh(std::string file);

	bool ChackIsNearMesh(NavMesh *lm, NavMesh *rm);

	void InitNavMeshes();

public:
	//ナビメッシュコンテナ
	std::vector<NavMesh> navMeshData;


};