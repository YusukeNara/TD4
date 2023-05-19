#include "Result.h"

Result::Result(ISceneChanger *changer)
{
	groundTex = TexManager::LoadTexture("Resources/asp3.png");

	groundObject = std::make_shared<Object3d>();
	groundObject.reset(NY_Object3DManager::Get()->CreateModel_Tile(500.f, 500.f, 10.f, 10.f, groundTex));

	groundObject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));
}

Result::~Result()
{
}

void Result::Initialize()
{
}

void Result::Finalize()
{

}

void Result::Update()
{
	float a = ScoreManager::GetScore();
	float b = ScoreManager::GetReview();
}

void Result::Draw()
{
	NY_Object3DManager::Get()->SetRestartObject3D();

	//床
	groundObject->DrawObject();

	NY_Object3DManager::Get()->CloseDrawObject3D();
}

void Result::Draw2D()
{
}
