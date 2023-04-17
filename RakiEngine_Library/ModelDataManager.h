#pragma once
#include "NY_Model.h"
#include "FbxLoader.h"

#include <string>
#include <vector>

class ModelDataManager
{
public:

	static int LoadOBJModel(const char *modelname);

	static int LoadFBXModel(const char *modelname);




private:
	//モデルデータコンテナ
	std::vector<Model3D> models;

	std::vector<fbxModel> fbxModels;




};