#include "EngineDebugScene.h"

EngineDebugScene::EngineDebugScene(ISceneChanger* changer)
{
	UINT testTex = TexManager::LoadTexture("Resources/asp3.png");

	testobject = std::make_shared<Object3d>();
	testobject.reset(NY_Object3DManager::Get()->CreateModel_Box(50.f, 10.f, 10.f, testTex));

	testobject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));

	testsp.Create(testTex);

	testFBX_YesBone = std::make_shared<Object3d>();
	testFBX_YesBone.reset(LoadModel_FBXFile("hageBoonYes"));
	testFBX_YesBone->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(-50.f, 0, 0));

	testFBX_NoBone = std::make_shared<Object3d>();
	testFBX_NoBone.reset(LoadModel_FBXFile("hageBoonNo"));
	testFBX_NoBone->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(50.f, 0, 0));

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

}

void EngineDebugScene::Draw()
{
	//testobject->DrawObject();
	testFBX_NoBone->DrawObject();
	testFBX_YesBone->DrawObject();

}

void EngineDebugScene::Draw2D()
{

}