#include "HageHead.h"

HageHead::HageHead()
{
	headOffset = RVector3(0, 10.f, 0);
}

HageHead::~HageHead()
{
}

void HageHead::Init()
{
	hageheadTex = TexManager::LoadTexture("Resources/blackParticleTex.png");

	headObject = std::make_shared<Object3d>();

	headObject.reset(NY_Object3DManager::Get()->CreateModel_Box(5.f, 1.f, 1.f, hageheadTex));

	scale = RVector3(1, 1, 1);
	rot = RVector3(0, 0, 0);
	pos.zero();
	headObject->SetAffineParam(scale, rot, pos);
	isHairDestroy = true;
}

void HageHead::Update()
{
	if (isactive)
	{
		SlappingMove();
	}
}

void HageHead::Draw()
{
	headObject->DrawObject();
}

void HageHead::Finalize()
{
}

void HageHead::SlappingMove()
{
	if (!isHairDestroy)
	{
		return;
	}

	//プレイヤーの入力を受け付けたら
	//if(ptr->)
	//{}

	//アニメーションしてふっとんっでいく処理

	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A))
	{
		isAllMoveFinish = true;

	}

}
