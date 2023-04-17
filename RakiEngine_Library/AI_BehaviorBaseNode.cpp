
#include "AI_BehaviorBaseNode.h"
#include "Behavior_JudgeBase.h"
#include "NY_random.h"

#include <d3d12.h>
#include "Raki_imguiMgr.h"

void BehaviorBaseNode::CreateJudgeNode(std::string nodeName, SELECT_RULE rule, BehaviorJudgeBase* judgeObject)
{
    //ノードの名前
    this->nodeName = nodeName;
    //ルール
    this->rule = rule;
    //判定オブジェクト
    this->judgeObject = judgeObject;
    //ノードの種類
    type = NODE_TYPE::TYPE_SELECTER;
}

void BehaviorBaseNode::AddjudgeNodeChild(BehaviorBaseNode* child)
{
    //子ノードに自身を親として登録
    child->parent = this;
    //子ノード追加
    childs.push_back(child);
}

void BehaviorBaseNode::CreateActionNode(std::string nodeName, BehaviorActionBase* actObject,BehaviorJudgeBase *judgeObject)
{
    if (actObject == nullptr) { return; }

    this->nodeName = nodeName;

    this->actObject = actObject;

    this->judgeObject = judgeObject;

    type = NODE_TYPE::TYPE_EXECUTE;
}

BehaviorBaseNode* BehaviorBaseNode::Inference()
{
    //子ノードリスト
    std::vector<BehaviorBaseNode*> list;
    //配列サイズ確保
    list.reserve(childs.size());
    //返却用変数 
    BehaviorBaseNode* result = nullptr;

    //子ノードが選択可能かを確認
    for (auto &n : childs)
    {
        //判定スクリプトがnullの場合は無条件で格納
        if (n->judgeObject == nullptr) {
            list.push_back(n);
        }
        //子ノードの判定クラスがtrueのとき
        else if (n->judgeObject->Judge()){
            //候補リストに格納
            list.push_back(n);
        }
    }

    if (list.size() == size_t(0) ) {
        //一つも当てはまらない場合、すべてのノードを入れる
        //ただし意図しない場合もあるので、オプションで設定可能にする
        for (auto& n : childs)
        {
            //候補リストに格納
            list.push_back(n);
        }
    }

    //候補リストの中から選択条件をもとに返すノードを決定
    switch (rule)
    {
    case BehaviorBaseNode::RULE_RANDOM:
        //ランダムにノードを決定
        result = Select_Random(&list);
        return result;
        break;

    case BehaviorBaseNode::RULE_PRIORITY:
        //優先度でノード決定
        result = Select_Priority(&list);
        return result;
        break;

    case BehaviorBaseNode::RULE_ONOFF:
        //ランダムにノードを決定
        result = Select_Random(&list);
        return result;
        break;

    case BehaviorBaseNode::RULE_SEAQUENCE:
        //ランダムにノードを決定
        result = Select_Random(&list);
        return result;
        break;
    default:
        //ランダムにノードを決定
        result = Select_Random(&list);
        return result;
        break;
    }

    //ランダムにノードを決定
    result = Select_Random(&list);
    return result;
}

BehaviorActionBase::ACTION_STATE BehaviorBaseNode::Run()
{
    //行動実行
    return actObject->Run();
}

BehaviorBaseNode *BehaviorBaseNode::Select_Random(std::vector<BehaviorBaseNode*> *lists)
{
    int selectNumber = NY_random::intrand_sl(static_cast<int>(lists->size() - 1), 0);

    return (*lists)[selectNumber];
}

BehaviorBaseNode* BehaviorBaseNode::Select_Priority(std::vector<BehaviorBaseNode*>* lists)
{
    BehaviorBaseNode* result = nullptr;

    unsigned int priority = -100;

    for (auto itr = lists->begin(); itr != lists->end(); itr++) {
        if (priority < (*itr)->priority) {
            result = (*itr);
            priority = (*itr)->priority;
        }
    }

    return result;
}

void BehaviorBaseNode::DrawNodeInfo(std::vector<BehaviorBaseNode*> editNodes, std::vector<BehaviorActionBase*> actObjects, 
    std::vector<BehaviorJudgeBase*> judgeObjects, bool checkIsDisplay)
{
    //デバッグ用、簡易的だが編集も可能に

    //表示フラグが立ってない
    if (checkIsDisplay && !isDisplay) { return; }
    

    //編集用変数
    int nowSelectRule = static_cast<int>(rule);
    std::string parentName;
    if (parent != nullptr) {
        parentName = parent->nodeName;
    }


    ImguiMgr::Get()->StartDrawImgui(nodeName.c_str(), 100, 300);

    if (ImGui::Button("Close Node Info")) { isDisplay = false; }

    switch (type)
    {
    case BehaviorBaseNode::TYPE_EXECUTE:
        ImGui::Text("Node Type : EXECUTE\n");

        ImGui::Text("Act script name : %s", actObject->actScriptName.c_str());

        ImGui::Text("Judge script Name : %s", judgeObject->judgeScriptName.c_str());

        ImGui::Text("Node Status\n");

        if (actObject != nullptr) {
            switch (actObject->actionState)
            {
            case BehaviorActionBase::ACTION_STATE::STANDBY:
                ImGui::Text("Process is standby.");
                break;

            case BehaviorActionBase::ACTION_STATE::RUNNING:
                ImGui::Text("Process is running.");
                break;

            case BehaviorActionBase::ACTION_STATE::SUCCESS:
                ImGui::Text("Process is successed.");
                break;

            case BehaviorActionBase::ACTION_STATE::FAILED:
                ImGui::Text("Process is failed.");
                break;

            default:
                break;
            }
        }

        ImGui::Text(parentName.c_str());


        break;
    case BehaviorBaseNode::TYPE_SELECTER:
        ImGui::Text("Node Type : SELECTER");

        ImGui::Text("Select rules");

        ImGui::RadioButton("Random", &nowSelectRule, static_cast<int>(RULE_RANDOM));
        ImGui::SameLine();
        ImGui::RadioButton("Priority", &nowSelectRule, static_cast<int>(RULE_PRIORITY));

        break;
    default:
        break;
    }


    ImGui::Checkbox("Edit Scripts Pointer", &isChangeScripts);
    if (isChangeScripts) {
        BehaviorActionBase* acttemp = nullptr;
        ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(150, 250), ImGuiWindowFlags_NoTitleBar);
        for (auto& acts : actObjects) {
            if (ImGui::Button(acts->actScriptName.c_str())) {
                acttemp = acts;
            }
        }
        ImGui::EndChild();

        if (acttemp != nullptr) { this->actObject = acttemp; }

        BehaviorJudgeBase* judgetemp = nullptr;
        ImGui::BeginChild(ImGui::GetID((void*)1), ImVec2(150, 250), ImGuiWindowFlags_NoTitleBar);
        for (auto& judges : judgeObjects) {
            if (ImGui::Button(judges->judgeScriptName.c_str())) {
                judgetemp = judges;
            }
        }
        ImGui::EndChild();

        if (judgetemp != nullptr) { this->judgeObject = judgetemp; }
    }

    //DrawNodeInfo_Child(editNodes, actObjects, judgeObjects);

    ImguiMgr::Get()->EndDrawImgui();

    //設定適用
    rule = static_cast<SELECT_RULE>(nowSelectRule);
}

void BehaviorBaseNode::DrawNodeInfo_Child(std::vector<BehaviorBaseNode*> editNodes, std::vector<BehaviorActionBase*> actObjects, std::vector<BehaviorJudgeBase*> judgeObjects)
{
    if (this->isDisplay) {
        for (auto& child : childs) {
            ImGui::BeginChild(ImGui::GetID((void*)2), ImVec2(150, 250), ImGuiWindowFlags_NoTitleBar);
            child->DrawNodeInfo_withEditor(editNodes, actObjects, judgeObjects);
            ImGui::EndChild();
        }
    }
}

void BehaviorBaseNode::DrawNodeInfo_withEditor(std::vector<BehaviorBaseNode*> editNodes, std::vector<BehaviorActionBase*> actObjects, std::vector<BehaviorJudgeBase*> judgeObjects)
{
    //編集用変数
    int nowSelectRule = static_cast<int>(rule);
    std::string parentName;
    if (parent != nullptr) {
        parentName = parent->nodeName;
    }

    switch (type)
    {
    case BehaviorBaseNode::TYPE_EXECUTE:
        ImGui::Text("Node Type : EXECUTE\n");

        ImGui::Text("Node Status\n");

        if (actObject != nullptr) {
            switch (actObject->actionState)
            {
            case BehaviorActionBase::ACTION_STATE::STANDBY:
                ImGui::Text("Process is standby.");
                break;

            case BehaviorActionBase::ACTION_STATE::RUNNING:
                ImGui::Text("Process is running.");
                break;

            case BehaviorActionBase::ACTION_STATE::SUCCESS:
                ImGui::Text("Process is successed.");
                break;

            case BehaviorActionBase::ACTION_STATE::FAILED:
                ImGui::Text("Process is failed.");
                break;

            default:
                break;
            }
        }

        ImGui::Text(parentName.c_str());

        break;
    case BehaviorBaseNode::TYPE_SELECTER:
        ImGui::Text("Node Type : SELECTER");

        ImGui::Text("Select rules");

        ImGui::RadioButton("Random", &nowSelectRule, static_cast<int>(RULE_RANDOM));
        ImGui::SameLine();
        ImGui::RadioButton("Priority", &nowSelectRule, static_cast<int>(RULE_PRIORITY));
        ImGui::SameLine();

        break;
    default:
        break;
    }
}
