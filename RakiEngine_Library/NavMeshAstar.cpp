#include "NavMeshAstar.h"

#include <algorithm>
#include <iostream>

void NavMeshAstar::SetNavMeshData(std::vector<NavMesh> &navMeshes)
{
	for (auto& nm : navMeshes) {
		navMeshData.push_back(&nm);
	}
}

void NavMeshAstar::NavMeshSearchAstar(NavMesh *start, NavMesh *goal, std::vector<RVector3> &result)
{
	//ステージ情報を初期化(ヒューリスティックコストを再計算)
	for (int i = 0; i < navMeshData.size(); i++) {
		navMeshData[i]->InitCost();
	}

	//rootDataを空に
	rootData.clear();

	//a*方式でナビメッシュの経路探索を実行

	std::vector<NavMesh*> openMesh;
	std::vector<NavMesh*> closeMesh;
	NavMesh* last_update_mesh = nullptr;

	//スタートメッシュをopenMeshに追加
	openMesh.push_back(start);

	//スタート位置のコスト計算
	start->heuristicCost = CaclHeuristicCost(goal, start);
	start->totalCost = start->heuristicCost;


	//openmeshがなくなるまでループ
	while (!openMesh.empty()) {

		//openmeshから取得
		auto searcingMesh = (*openMesh.begin());
		openMesh.erase(openMesh.begin());

		//ゴールなら終了
		if (searcingMesh->indexNumStr == goal->indexNumStr) {
			closeMesh.push_back(searcingMesh);
			break;
		}
		//そうでないなら、取得メッシュの隣接メッシュをオープンリストに
		for (auto &m : searcingMesh->navmeshptr) {
			int hctmp = -1, tctmp = 0;
			//このとき、ヒューリスティックコストがないなら計算する
			if (m->heuristicCost < 0) {
				m->heuristicCost = CaclHeuristicCost(goal, m);
			}

			tctmp = m->heuristicCost + searcingMesh->totalCost;

			//追加判定を実行し、okならopenに追加
			if (CheckNavMeshAddOpenAdd(openMesh, closeMesh, m, tctmp)) {
				//トータルコストに代入
				m->totalCost = tctmp;
				openMesh.push_back(m);
				//ソート
				std::sort(openMesh.begin(), openMesh.end(), [](const auto& lm, const auto& rm) {
					return lm->totalCost < rm->totalCost;
					});
				//最終更新を保存
				last_update_mesh = m;
			}
		}

		bool add_close = true;
		//クローズに同じメッシュがあるか確認
		for (auto itr = closeMesh.begin(); itr != closeMesh.end(); itr++) {
			if ((*itr) == searcingMesh) {
				add_close = false;
			}
		}
		if (add_close) {
			closeMesh.push_back(searcingMesh);
		}
	}

	//経路が見つからなかった場合は終了
	if (last_update_mesh == nullptr) {
		return;
	}
	//openが空で終わった場合は終了
	if (openMesh.empty()) {
		return;
	}


	//経路復元
	std::vector<NavMesh*> result_route;

	NavMesh *resultSearcingMesh = last_update_mesh;
	rootData.push_back(resultSearcingMesh);
	//スタート位置まで戻る
	while (1) {
		
		NavMesh* nextMesh = nullptr;

		if (resultSearcingMesh->indexNumStr == start->indexNumStr) {
			rootData.push_back(resultSearcingMesh);

			break;
		}
		else {
			//隣接メッシュにゴールがある

			//隣接メッシュで最もコストが少ないメッシュをリストに
			 
			//トータルが計算されていない、すでに通ったメッシュじゃない
			std::vector<NavMesh*> contenderMesh;
			for (auto itr = resultSearcingMesh->navmeshptr.begin(); 
				itr != resultSearcingMesh->navmeshptr.end(); itr++) {
				if ((*itr)->totalCost > 0 && 
					std::find(rootData.begin(), rootData.end(),(*itr)) == rootData.end()) {
					contenderMesh.push_back((*itr));
				}
			}

			if (contenderMesh.empty()) {
				std::cout << "ERROR : NavMeshAstar : Route not found" << std::endl;
				return;
			}
			
			//ソートして先頭を取得
			//std::sort(contenderMesh.begin(), contenderMesh.end(), [](const auto& lm, const auto& rm) {
			//	return lm->totalCost < rm->totalCost;
			//	});
			//std::sort(contenderMesh.begin(), contenderMesh.end());

			NavMesh* m = contenderMesh[0];
			for (auto& cm : contenderMesh) {
				//通過済メッシュリストになくて、コストを更新する
				if (cm->totalCost < m->totalCost) {
					m = cm;
				}
			}

			//判定済メッシュ

			//auto minmesh = *std::max_element(contenderMesh.begin(), contenderMesh.end());

			rootData.push_back(m);
			resultSearcingMesh = m;
		}
	}

	//for (auto& nm : rootData) {
	//	result.push_back(*nm);
	//}

	for (auto& nm : rootData) {
		result.push_back(nm->center);
	}

	//std::copy(result_route.begin(), result_route.end(), rootData.begin());

	return;
}

void NavMeshAstar::NavMeshSearchAster(RVector3 nowPos, RVector3 targetPos, std::vector<RVector3>& result)
{
	//座標からメッシュを検索
	NavMesh* startMesh = nullptr, * goalMesh = nullptr;
	startMesh = NowStandingMesh(nowPos);
	goalMesh = NowStandingMesh(targetPos);

	NavMeshSearchAstar(startMesh, goalMesh, result);
}

bool NavMeshAstar::CheckNavMeshAddOpenAdd(std::vector<NavMesh*> &open, std::vector<NavMesh*> &close, NavMesh* checkMesh, int cost)
{
	auto c_result = std::find_if(close.begin(), close.end(), [&](NavMesh* m) {return m->indexNumStr == checkMesh->indexNumStr; });
	auto o_result = std::find_if(open.begin(), open.end(), [&](NavMesh* m) {return m->indexNumStr == checkMesh->indexNumStr; });
	if (c_result == close.end() && o_result == open.end()) {
		return true;
	}
	else if (c_result != close.end()) {
		if ((*c_result)->totalCost > cost) {
			close.erase(c_result);
			return true;
		}
		else {
			return false;
		}
	}
	else if (o_result != open.end()) {
		if ((*o_result)->totalCost > cost) {
			open.erase(o_result);
			return true;
		}
		else {
			return false;
		}
	}

	//ここにくることはありえない
	return false;
}

int NavMeshAstar::CaclHeuristicCost(NavMesh* goalMesh, NavMesh* targetMesh)
{
	//元ノードトータルコスト + ノード間移動コスト + ヒューリスティックコスト = トータルコスト
	int result = int(distance(goalMesh->center, targetMesh->center));

	return result;
}

RVector3 NavMeshAstar::MoveWaypointDirectLine(float permissible, RVector3& nowpos, bool &isMoved)
{
	if (rootData.empty()) {
		std::cout << "ERROR : NavMeshAstar : Route not found" << std::endl;
		return RVector3(0, 0, 0);
	}

	//誤差値使わない気がする
	//todo 使わなくてよくなったら消す

	RVector3 result(0.0f, 0.0f, 0.0f);

	//現在いるメッシュを検索
	NavMesh *nowStand;
	nowStand = NowStandingMesh(nowpos);

	//ゴール地点のとき
	if (nowStand == *(rootData.begin())) {
		isMoved = true;
		return result;
	}

	auto itr = std::find(rootData.begin(), rootData.end(), nowStand);

	//ルート上に存在
	if (itr != rootData.end()) {
		//インデックス番号を取得
		int indexNum = int(std::distance(rootData.begin(), itr));
		//次のインデックスの方向を取得
		int nextIndexNum = indexNum - 1;
		//方向を計算
		result = rootData[nextIndexNum]->center - nowpos;
		result = result.norm();
	}
	else {
		//そうでないなら末尾ノードへ
		//でも末尾へ向かうルートを再計算しないといけない気がする...
		RVector3 beginPos = (*rootData.end() - 1)->center;
		result = beginPos - nowpos;
		result = result.norm();
	}


	return result;
}

RVector3 NavMeshAstar::MoveWaypointDirectLine(std::vector<NavMesh>& root, RVector3& nowpos)
{
	RVector3 result(0.0f, 0.0f, 0.0f);

	//現在いるメッシュを検索
	NavMesh *nowStand;
	nowStand = NowStandingMesh(nowpos);

	//ゴール地点のとき
	if (nowStand == *(root.begin())) {
		return result;
	}

	auto itr = std::find(root.begin(), root.end(), nowStand);

	//ルート上に存在
	if (itr != root.end()) {
		//インデックス番号を取得
		int indexNum = int(std::distance(root.begin(), itr));
		//次のインデックスの方向を取得
		int nextIndexNum = indexNum - 1;
		//方向を計算
		result = root[nextIndexNum].center - nowpos;
		result = result.norm();
	}
	else {
		//そうでないなら末尾ノードへ
		//でも末尾へ向かうルートを再計算しないといけない気がする...


	}


	return result;
}

RVector3 NavMeshAstar::MoveWaypointDirectLine(std::vector<RVector3>& root, RVector3& nowpos, float permissible, int& index, bool& usMoved)
{
	RVector3 result(0.0f, 0.0f, 0.0f);

	//範囲外
	if (index < 0 || index > root.size()) { return result; }

	//指し示しているインデックスをもとにベクトルを求める
	result = root[index] - nowpos;
	result.norm();

	//もし現在位置と目的地の距離が誤差の範囲なら
	if (distance(root[index], nowpos) < permissible) {
		if (index == root.size()) {
			usMoved = true;
		}
		else {
			index++;
		}
	}

	return result;
}

NavMesh* NavMeshAstar::NowStandingMesh(RVector3 targetPos)
{

	float dist = distance(targetPos, navMeshData[0]->center);
	float comparition = 0;
	int targetIndex = 0;
	//一番近いナビメッシュを検索
	for (int i = 0; i < navMeshData.size();i++) {
		comparition = distance(targetPos, navMeshData[i]->center);
		if (dist > comparition) {
			dist = comparition;
			targetIndex = i;
		}
	}

	return navMeshData[targetIndex];
}
