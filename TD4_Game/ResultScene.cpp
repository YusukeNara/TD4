#include "ResultScene.h"
#include "ScoreManager.h"

ResultScene::ResultScene(ISceneChanger* changer) : BaseScene(changer)
{
	UINT testTex = TexManager::LoadTexture("Resources/asp3.png");

	resultPlayer = std::make_shared<Object3d>();
	resultPlayer.reset(LoadModel_FBXFile("hage_1"));
	resultPlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(70, 0, 0));
	resultPlayer->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 0);
	starPos = { 200.0f,100.0f };
	spriteScore.CreateAndSetDivisionUVOffsets(10, 10, 1, 64, 64, TexManager::LoadTexture("Resources/Score.png"));
	spriteReview.CreateAndSetDivisionUVOffsets(3, 3, 1, 128, 128, TexManager::LoadTexture("Resources/star.png"));
	starIsDraw = false;
	scoreTexIsDraw = false;
	totalScore = ScoreManager::GetScore();
	totalReview = ScoreManager::GetReview();
	handleNum = ScoreManager::GetHandle();
}

ResultScene::~ResultScene()
{
	if (resultPlayer) {
		std::cout << "player deleted" << endl;
	}
}

void ResultScene::Initialize()
{

}

void ResultScene::Finalize()
{

}

void ResultScene::Update()
{
	Animation();
}

void ResultScene::Draw()
{
	resultPlayer->DrawObject();
}

void ResultScene::Draw2D()
{
	//�g�[�^�����r���[�̕\��
	if (totalReview <= 5 && totalReview >= 4.75f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 4.25f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 1;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 3.75f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 3.25f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 1;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 2.75f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 2.25f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 1;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 1.75f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 1.25f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 1;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 0.75f)
	{
		spriteReview.uvOffsetHandle = 2;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else if (totalReview >= 0.25f)
	{
		spriteReview.uvOffsetHandle = 1;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	else
	{
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 64.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 128.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 192.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y + 64.f);
		spriteReview.uvOffsetHandle = 0;
		spriteReview.DrawExtendSprite(TOTALREVIEW_POS.x + 256.f, TOTALREVIEW_POS.y, TOTALREVIEW_POS.x + 320.f, TOTALREVIEW_POS.y + 64.f);
	}
	//�g�[�^���̃X�R�A�̕\��
	if (totalScore >= 1000000)
	{
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 384.f, 720.f - 64.f, SCORE_POS_X - 320.f, 720.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 320.f, 720.f - 64.f, SCORE_POS_X - 256.f, 720.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 256.f, 720.f - 64.f, SCORE_POS_X - 192.f, 720.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 192.f, 720.f - 64.f, SCORE_POS_X - 128.f, 720.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 128.f, 720.f - 64.f, SCORE_POS_X - 64.f, 720.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 64.f, 720.f - 64.f, SCORE_POS_X - 0.f, 720.f);
	}
	else if (totalScore >= 100000)
	{
		spriteScore.uvOffsetHandle = (int)totalScore / 100000 % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 384.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 320.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = (int)totalScore / 10000 % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 320.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 256.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = (int)totalScore / 1000 % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 256.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 192.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 192.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 128.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 128.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 64.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = (int)totalScore % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 64.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 0.f, SCORE_POS_Y);
	}
	else if (totalScore >= 10000)
	{
		spriteScore.uvOffsetHandle = (int)totalScore / 10000 % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 320.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 256.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = (int)totalScore / 1000 % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 256.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 192.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 192.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 128.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 128.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 64.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = (int)totalScore % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 64.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 0.f, SCORE_POS_Y);
	}
	else if (totalScore >= 1000)
	{
		spriteScore.uvOffsetHandle = (int)totalScore / 1000 % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 256.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 192.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 192.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 128.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 128.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 64.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = (int)totalScore % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 64.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 0.f, SCORE_POS_Y);
	}
	else
	{
		spriteScore.uvOffsetHandle = (int)totalScore / 100 % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 192.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 128.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = ((int)totalScore / 10) % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 128.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 64.f, SCORE_POS_Y);
		spriteScore.uvOffsetHandle = (int)totalScore % 10;
		spriteScore.DrawExtendSprite(SCORE_POS_X - 64.f, SCORE_POS_Y - 64.f, SCORE_POS_X - 0.f, SCORE_POS_Y);
	}
	//�J�����l��
	if (handleNum >= 1000)
	{
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 64.f, HANDLE_POS.y, HANDLE_POS.x + 128.f, HANDLE_POS.y + 64.f);
		spriteScore.uvOffsetHandle = 9;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 0.f, HANDLE_POS.y, HANDLE_POS.x + 64.f, HANDLE_POS.y + 64.f);
	}
	else if (handleNum >= 100)
	{
		spriteScore.uvOffsetHandle = (int)handleNum % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
		spriteScore.uvOffsetHandle = ((int)handleNum / 10) % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 64.f, HANDLE_POS.y, HANDLE_POS.x + 128.f, HANDLE_POS.y + 64.f);
		spriteScore.uvOffsetHandle = (int)handleNum / 100 % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 0.f, HANDLE_POS.y, HANDLE_POS.x + 64.f, HANDLE_POS.y + 64.f);
	}
	else if (handleNum >= 10)
	{
		spriteScore.uvOffsetHandle = (int)handleNum % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
		spriteScore.uvOffsetHandle = ((int)handleNum / 10) % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 64.f, HANDLE_POS.y, HANDLE_POS.x + 128.f, HANDLE_POS.y + 64.f);
	}
	else
	{
		spriteScore.uvOffsetHandle = (int)handleNum % 10;
		spriteScore.DrawExtendSprite(HANDLE_POS.x + 128.f, HANDLE_POS.y, HANDLE_POS.x + 192.f, HANDLE_POS.y + 64.f);
	}
	if (starIsDraw)
	{
		spriteReview.Draw();
	}
	if (scoreTexIsDraw)
	{
		spriteScore.Draw();
	}
}

void ResultScene::DrawImgui()
{

}

void ResultScene::Animation()
{
	if (animationCount < animationCountMax)
	{
		animationCount++;
	}
	if (animationCount > animationCountMax / 4)
	{
		starIsDraw = true;
	}
	if (animationCount > animationCountMax / 2)
	{
		scoreTexIsDraw = true;
	}
}