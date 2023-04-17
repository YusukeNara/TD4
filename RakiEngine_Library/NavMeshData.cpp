#include "NavMeshData.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

void NavMeshData::LoadNavMesh(std::string file)
{
	//FileStream
	std::ifstream stageFile;

	//objファイルオープン
	stageFile.open(file);
	//オープン失敗をチェック
	if (stageFile.fail() == true)
	{
		return;
	}
	
	std::vector<RVector3> vertexPos;
	std::vector<std::array<unsigned short, 3>> indexes;

	std::string line;

	while (std::getline(stageFile, line)) {

		//1行分の文字列をストリームに変換して読みやすくする
		std::istringstream line_stream(line);

		//半角スペースで行の先頭文字列取得
		string key;
		getline(line_stream, key, ' ');

		if (key == "v") {

			RVector3 loadedPos{};
			line_stream >> loadedPos.x;
			line_stream >> loadedPos.y;
			line_stream >> loadedPos.z;

			vertexPos.emplace_back(loadedPos);
		}

		if (key == "m") {
			string index_string;
			array<unsigned short, 3> index;
			while (getline(line_stream, index_string, ' ')) {
				istringstream index_stream(index_string);
				index_stream >> index[0];
				index_stream.seekg(1, ios_base::cur);
				index_stream >> index[1];
				index_stream.seekg(1, ios_base::cur);
				index_stream >> index[2];
			}

			indexes.push_back(index);
		}
	}

	//抽出した情報をナビメッシュ配列に導入

	//インデックス配列の数 = ナビメッシュの総数分回す
	for (int i = 0; i < indexes.size(); i++) {
		NavMesh mesh;

		for (int j = 0; j < 3; j++) {
			//メッシュ情報保持
			mesh.indices[j] = indexes[i][j];
			mesh.indexNumStr += (std::to_string(indexes[i][j]) + " ");
			mesh.navmashVertex[j] = vertexPos[indexes[i][j]];
		}
		mesh.center = CalcTriangleCenter(mesh.navmashVertex[0], mesh.navmashVertex[1], mesh.navmashVertex[2]);
		//末尾に挿入
		navMeshData.push_back(mesh);
	}

	//保存したナビメッシュのインデックスをもとに、各メッシュに隣接メッシュの情報を入れる
	for (int i = 0; i < navMeshData.size(); i++) {

		//メッシュインデックス3つのうち、2つが一致するメッシュを3枚検索

		//一枚目
		auto nm1 = std::find_if(navMeshData.begin(), navMeshData.end(), [&](
			NavMesh& rm) {return ChackIsNearMesh(&navMeshData[i], &rm); });
		if (nm1 != navMeshData.end()) { 
			navMeshData[i].navmeshptr.push_back(&(*nm1));
			nm1++;
		}

		//二枚目
		auto nm2 = std::find_if(nm1, navMeshData.end(), [&](
			NavMesh& rm) {return ChackIsNearMesh(&navMeshData[i], &rm); });
		if (nm2 != navMeshData.end()) { 
			navMeshData[i].navmeshptr.push_back(&(*nm2)); 
			nm2++;
		}

		//三枚目
		auto nm3 = std::find_if(nm2, navMeshData.end(), [&](
			NavMesh& rm) {return ChackIsNearMesh(&navMeshData[i], &rm); });
		if (nm3 != navMeshData.end()) {
			navMeshData[i].navmeshptr.push_back(&(*nm3));
		}
	}

}

bool NavMeshData::ChackIsNearMesh(NavMesh *lm, NavMesh *rm)
{
	std::array<bool, 3> isMatchIndex = { false,false,false };

	for (int i = 0; i < 3; i++) {
		auto idx = std::find(rm->indices.begin(), rm->indices.end(),lm->indices[i]);
		if (idx != rm->indices.end()) { isMatchIndex.at(i) = true; }
	}

	//3つのうち2つが一致
	return std::count(isMatchIndex.begin(), isMatchIndex.end(), true) == 2;
}

void NavMeshData::InitNavMeshes()
{
	for (auto& m : navMeshData) {
		m.InitCost();
	}
}
