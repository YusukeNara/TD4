#include "BehaviorEditor.h"

#include <d3d12.h>

BehaviorEditor::BehaviorEditor()
{
}

BehaviorEditor::~BehaviorEditor()
{
}

void BehaviorEditor::Init(AI_BehaviorTree* treePointer)
{
	this->treePointer = treePointer;
	nodes.push_back(treePointer->firstNode);
}

void BehaviorEditor::AddEditData_ActScript(BehaviorActionBase* actObject)
{
	//コンテナに追加
	actScripts.push_back(actObject);
}

void BehaviorEditor::AddEditData_JudgeScript(BehaviorJudgeBase* judgeObject)
{
	//コンテナに追加
	judgeScripts.push_back(judgeObject);
}

void BehaviorEditor::AddEditData_Node(BehaviorBaseNode* node)
{
	//ノード追加
	nodes.push_back(node);

	//ノードのオブジェクトを格納
	if (node->actObject != nullptr) {
		AddEditData_ActScript(node->actObject);
	}

	if (node->judgeObject != nullptr) {
		AddEditData_JudgeScript(node->judgeObject);
	}
}

BehaviorBaseNode* BehaviorEditor::CreateNewNode(std::string nodeName)
{
	BehaviorBaseNode* result = new BehaviorBaseNode;
	result->nodeName = nodeName;

	nodes.push_back(result);

	return result;
}

void BehaviorEditor::ObjectDataDraw()
{
	//データをリスト表示し、選択されたものの詳細を表示する
	ImguiMgr::Get()->StartDrawImgui("Behavior Objects", 200, 500);

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(200, 250), ImGuiWindowFlags_NoTitleBar);
	for (auto& nodelist : nodes) {
		if (ImGui::Button(nodelist->nodeName.c_str())) { 
			nodelist->isDisplay = true; 
		}
	}
	ImGui::EndChild();

	//ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(200, 250), ImGuiWindowFlags_NoTitleBar);
	//for (auto& judgelist : judgeScripts) {
	//	if(ImGui::Button(judgelist->judgeScriptName.c_str())){}
	//}
	//ImGui::EndChild();

	//ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(200, 250), ImGuiWindowFlags_NoTitleBar);
	//for (auto& actlist : actScripts) {
	//	ImGui::Button(actlist->actScriptName.c_str());
	//}
	//ImGui::EndChild();


	ImguiMgr::Get()->EndDrawImgui();
}

void BehaviorEditor::EditorDraw()
{
	//データをリスト表示し、選択されたものの詳細を表示する
	ImguiMgr::Get()->StartDrawImgui("BehaviorEditorData", 250, 500);

	ImGui::Text("Behavior Tree Data");

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(200, 250), ImGuiWindowFlags_NoTitleBar);

	treePointer->firstNode->DrawNodeInfo_withEditor(nodes, actScripts, judgeScripts);

	ImGui::EndChild();

	ImGui::Text("Root Node : %s", treePointer->rootNode->nodeName.c_str());

	//エディターウィンドウの描画終了
	ImguiMgr::Get()->EndDrawImgui();

	//データ一覧



}

void BehaviorEditor::NodeDataDraw()
{
	//表示フラグが立ってるノードを表示、編集
	for (auto& list : nodes) {
		if (list->isDisplay) { 
			//ノード
			list->DrawNodeInfo(nodes, actScripts, judgeScripts);
			//子ノード
			list->DrawNodeInfo_Child(nodes, actScripts, judgeScripts);
		}
	}
}
