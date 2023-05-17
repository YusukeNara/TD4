#include "EngineDebugScene.h"

#include <Raki_imguiMgr.h>

using namespace Rv3Ease;

EngineDebugScene::EngineDebugScene(ISceneChanger* changer)
{
	UINT testTex = TexManager::LoadTexture("Resources/asp3.png");

	testobject = std::make_shared<Object3d>();
	testobject.reset(NY_Object3DManager::Get()->CreateModel_Box(10.f, 1.f, 1.f, testTex));
	testobject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));
	testEase.Init(RV3_EASE_TYPE::EASE_CUBIC_INOUT, RVector3(0, 0, 0),
		RVector3(0, 50, 0), 30);


	testsp.Create(testTex);

	testFBX_YesBone = std::make_shared<Object3d>();
	testFBX_YesBone.reset(LoadModel_FBXFile("hage_1"));
	testFBX_YesBone->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(90, 0, 0), RVector3(-50.f, 0, 0));
	testFBX_YesBone->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP,1);

	testFBX_NoBone = std::make_shared<Object3d>();
	testFBX_NoBone.reset(LoadModel_FBXFile("hageBoonNo"));
	testFBX_NoBone->SetAffineParam(RVector3(0.002f, 0.002f, 0.002f), RVector3(90, 0, 0), RVector3(50.f, 0, 0));

	RVector3 eye(0.f, 0.f, -200.f);
	RVector3 target(0.f, 0.f, 0.f);
	RVector3 up(0.f, 1.f, 0.f);
	NY_Camera::Get()->SetViewStatusEyeTargetUp(eye, target, up);
}

EngineDebugScene::~EngineDebugScene()
{
}

void EngineDebugScene::Initialize()
{
}

void EngineDebugScene::Finalize()
{
}

void EngineDebugScene::Update()
{
	if (Input::isKeyTrigger(DIK_Q)) { testEase.Play(); }
	else if (Input::isKeyTrigger(DIK_E)) { testEase.Reset(); }

	testobject->SetAffineParamTranslate(testEase.Update());
}

void EngineDebugScene::Draw()
{
	testobject->DrawObject();

	testFBX_NoBone->DrawObject();
	testFBX_YesBone->DrawObject();

}

void EngineDebugScene::Draw2D()
{

}

void EngineDebugScene::DrawImgui()
{
	int oldmode = testmode;

	myImgui::StartDrawImGui("fbx control", 150, 700);

	ImGui::SliderFloat("rotX", &rotX, 0.f, 360.f);
	ImGui::SliderFloat("rotY", &rotY, 0.f, 360.f);
	ImGui::SliderFloat("rotZ", &rotZ, 0.f, 360.f);

	myImgui::EndDrawImGui();

	testFBX_NoBone->SetAffineParamRotate(RVector3(rotX, rotY, rotZ));
	testFBX_YesBone->SetAffineParamRotate(RVector3(rotX, rotY, rotZ));
}
