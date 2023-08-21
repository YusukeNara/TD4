#include "EngineDebugScene.h"

#include "GameSoundMgr.h"

#include <Raki_imguiMgr.h>
#include <DirectionalLight.h>

#include <FbxLoader.h>

#include <RakiUtility.h>
#include <NY_random.h>



using namespace Rv3Ease;

EngineDebugScene::EngineDebugScene(ISceneChanger* changer, SceneChangeDirection* scDirect) : BaseScene(changer, scDirect)
{
	UINT testTex = TexManager::LoadTexture("Resources/asp3.png");

	testobject = std::make_shared<Object3d>();
	testobject.reset(NY_Object3DManager::Get()->CreateModel_Box(10.f, 1.f, 1.f, testTex));
	testobject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));
	testEase.Init(RV3_EASE_TYPE::EASE_CUBIC_INOUT, RVector3(0, 0, 0),
		RVector3(0, 50, 0), 30);

	testsp1.Create(testTex);
	testsp2.Create(testTex);

	

	testFBX_NoBone = std::make_shared<Object3d>();
	std::shared_ptr<fbxModel> testModel = std::make_shared<fbxModel>();
	testModel.reset(FbxLoader::GetInstance()->LoadFBXFile("SpiralPBR"));
	testFBX_NoBone.reset(SetModel_FBX(testModel));
	testFBX_NoBone->SetAffineParam(RVector3(20.1f, 20.1f, 20.1f), RVector3(90, 0, 0), RVector3(-75.0f, 25, -50.0f));

	testFBX_YesBone = std::make_shared<Object3d>();
	testFBX_YesBone.reset(SetModel_FBX(testModel));
	testFBX_YesBone->SetAffineParam(RVector3(0.1f, 0.1f, 0.1f), RVector3(90, 0, 0), RVector3(75.f, 0, -50.f));

	testobj = std::make_shared<Object3d>();
	testobj.reset(LoadModel_FBXFile("SpherePBR"));
	testobj->SetAffineParam(RVector3(20.1f, 20.1f, 20.1f), RVector3(90, 0, 0), RVector3(0.f, 25, -50.f));

	RVector3 eye(0.f, 50.f, -200.f);
	RVector3 target(0.f, 0.f, 0.f);
	RVector3 up(0.f, 1.f, 0.f);
	NY_Camera::Get()->SetViewStatusEyeTargetUp(eye, target, up);

	//音ロード
	testSE.reset(Audio::LoadSound_wav("Resources/don.wav"));
	testBGM = std::make_unique<SoundData>();
	testBGM.reset(Audio::LoadSound_wav("Resources/sounds/bgm/titlebgm.wav"));

	testNum.CreateAndSetDivisionUVOffsets(10, 10, 1, 64, 64, TexManager::LoadTexture("Resources/Score.png"));

	//無限ループ
	Audio::SetPlayRoopmode(testBGM.get(), 255);

	q1 = quaternion(1, 2, 3, 4);
	q2 = quaternion(2, 3, 4, 1);

	q1 *= q2;

	lightdir = RVector3(0, 0, 1);

	testp = std::make_unique<ParticleManager>();
	testp.reset(ParticleManager::Create(false,ParticleBlendState::PBLEND_MODE_MIN));
	particleTex = TexManager::LoadTexture("Resources/effect1.png");
	pgstate.scale_end = 0.0f;
	pgstate.scale_start = 10.0f;
	pgstate.position = RVector3(0, 50, 0);
	pgstate.isRandomSpawn = true;
	pgstate.position_spawnRange1 = RVector3(50.f, 50.f, 50.f);
	pgstate.position_spawnRange1 = RVector3(-50.f, -50.f, -50.f);
	pgstate.aliveTime = 60;
	pgstate.color_start = { 1.f,1.f,1.f,1.f };
	pgstate.color_end = { 1.f,1.f,1.f,1.f };

	controlPoint[1] = RVector3(50, 50, 0);
	controlPoint[2] = RVector3(0, 0, 0);
	controlPoint[3] = RVector3(-50, 50, 0);
	controlPoint[4] = RVector3(0, 100, 0);
	controlPoint[0] = controlPoint[4];
	controlPoint[5] = controlPoint[1];

	testspline.Init(controlPoint.data(), 6, 240);

	testbezier.Init(RVector3(-100, 0, 0), RVector3(0, 100, 100), RVector3(0, 0, 0),
		60, RV3_EASE_TYPE::EASE_CUBIC_OUT);

	mSceneChangeDirection->PlayInDirection();
}

EngineDebugScene::~EngineDebugScene()
{
}

void EngineDebugScene::Initialize()
{
	testcam.Init(RVector3(0, 0, 0), RVector3(0, 0, 1), RVector3(0, 1, 0));

	//test_ranking.Init();

	//test_ranking.PostScore(10);

	//test_ranking.GetRanking();
}

void EngineDebugScene::Finalize()
{
}

void EngineDebugScene::Update()
{
	if (Input::isKeyTrigger(DIK_Q)) { testspline.Play(); }
	else if (Input::isKeyTrigger(DIK_E)) { testspline.Reset(); }

	testobject->SetAffineParamTranslate(testspline.Update());

	if (Input::isKeyTrigger(DIK_O)) { Audio::PlayLoadedSound(testSE.get(), true); }

	if (Input::isKey(DIK_G)) { 
		//設定構造体のインスタンス
		ParticleGrainState pgstate{};
		RVector3 v(NY_random::floatrand_sl(30, -30), NY_random::floatrand_sl(30, -10), NY_random::floatrand_sl(30, -30));
		v = v.norm();
		//パラメータ設定
		pgstate.position = RVector3(5, 0, 0);
		pgstate.position = RVector3(5, 0, 0);
		pgstate.vel = v * 5.0f;
		pgstate.acc = RVector3(0, -0.2f, 0);
		pgstate.color_start = XMFLOAT4(0.3, 0.3, 0.3, 1);
		pgstate.color_end = XMFLOAT4(0.3, 0.3, 0.3, 1);
		pgstate.scale_start = 10.0f;
		pgstate.scale_end = 10.5f;
		pgstate.aliveTime = 60;
		pgstate.rot_start = 0.0f;
		pgstate.rot_end = 0.0f;
		testp->Add(pgstate);
	}

	if (Input::isKeyTrigger(DIK_R)) {
		test_ranking.StartDisplayResults();
	}

	//ランキングデータテスト
	//test_ranking.Update();

	//音テスト

}

void EngineDebugScene::Draw()
{
	//testobject->DrawObject();
	testobj->DrawObject();

	testFBX_NoBone->DrawObject();
	//testFBX_YesBone->DrawObject();

	
}

void EngineDebugScene::Draw2D()
{
	//testsp1.DrawExtendSprite(0, 0, 100, 100, { 1,1,1,0.5 });
	//testsp2.DrawExtendSprite(100, 100, 100, 100, { 1,1,1,1 });

	//ランキングデータ描画
	//test_ranking.Draw2D();

	
}

void EngineDebugScene::DrawImgui()
{
	int oldmode = testmode;

	myImgui::StartDrawImGui("fbx control", 150, 700);

	ImGui::SliderFloat("rotX", &rotX, 0.f, 360.f);
	ImGui::SliderFloat("rotY", &rotY, 0.f, 360.f);
	ImGui::SliderFloat("rotZ", &rotZ, 0.f, 360.f);

	ImGui::SliderInt("Animation num", &activeAnimation, 0, 8);
	ImGui::SliderInt("diff anim num", &differentAnimation, 0, 8);
	ImGui::Checkbox("isRoop", &isRoop);

	if (ImGui::Button("Play animation")) {
		//if(isRoop){ 
		//	testFBX_NoBone->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, activeAnimation); 
		//	testFBX_YesBone->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, differentAnimation);
		//}
		//else{ 
		//	testFBX_YesBone->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_FIRST, differentAnimation); 
		//	testFBX_NoBone->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, activeAnimation);
		//}
		
	}

	//ImGui::Text("test cam eye %.2f,%.2f,%.2f", testcam._eyepos.x, testcam._eyepos.y, testcam._eyepos.z);
	//ImGui::Text("test cam target %.2f,%.2f,%.2f", testcam._targetVec.x, testcam._targetVec.y, testcam._targetVec.z);
	//ImGui::Text("test cam up %.2f,%.2f,%.2f", testcam._upVec.x, testcam._upVec.y, testcam._upVec.z);
	//ImGui::Text("test cam up %.2f,%.2f,%.2f,%.2f", q1.x, q1.y, q1.z, q1.w);
	//ImGui::Text("lightdir %.2f,%.2f,%.2f", DirectionalLight::GetLightDir().x,
	//	DirectionalLight::GetLightDir().y, DirectionalLight::GetLightDir().z);
	//ImGui::SliderFloat("camrot", &camrot, 0.f, 2.0f);
	//ImGui::SliderInt("num", &testNum.uvOffsetHandle, 0, 9);
	//ImGui::SliderInt("dValue", &dval, -200000000, 200000000);
	myImgui::EndDrawImGui();

	testcam.Init(RVector3(0, 0, 0), RVector3(0, 0, 1), RVector3(0, 1, 0), camrot);



	myImgui::StartDrawImGui("light ctrl", 150, 300);

	ImGui::SliderFloat("light x", &lightdir.x, -1.f, 1.0f);
	ImGui::SliderFloat("light y", &lightdir.y, -1.f, 1.0f);
	ImGui::SliderFloat("light z", &lightdir.z, -1.f, 1.0f);

	DirectionalLight::SetLightDir(lightdir.x, lightdir.y, lightdir.z);

	myImgui::EndDrawImGui();



	myImgui::StartDrawImGui("Audio Control", 150, 300);

	if (ImGui::Button("PLAY")) {
		Audio::PlayLoadedSound(testBGM.get());
	}
	if (ImGui::Button("STOP")) {
		Audio::StopLoadedSound(testBGM.get());
	}
	if (ImGui::Button("PAUSE")) {
		Audio::PauseLoadedSound(testBGM.get());
	}

	static float masterVolume = 0.5f;
	ImGui::SliderFloat("master volume", &masterVolume, 0.0f, 1.0f);
	Audio::SetMasterVolume(masterVolume);

	myImgui::EndDrawImGui();

	testFBX_NoBone->SetAffineParamRotate(RVector3(rotX, rotY, rotZ));
	testFBX_YesBone->SetAffineParamRotate(RVector3(rotX, rotY, rotZ));
	testobj->SetAffineParamRotate(RVector3(rotX, rotY, rotZ));
	testobject->SetAffineParamRotate(RVector3(rotX, rotY, rotZ));


	//myImgui::StartDrawImGui("SOUND TEST", 100, 300);

	//if (ImGui::Button("PLAY TITLE BGM")) {
	//	GameSoundMgr::get()->PlayTitleBGM();
	//}
	//if (ImGui::Button("PLAY GAME BGM")) {
	//	GameSoundMgr::get()->PlayGameBGM();
	//}
	//if (ImGui::Button("PLAY RESULT BGM")) {
	//	GameSoundMgr::get()->PlayResultBGM();
	//}
	//if (ImGui::Button("STOP TITLE BGM")) {
	//	GameSoundMgr::get()->StopTitleBGM();
	//}
	//if (ImGui::Button("STOP GAME BGM")) {
	//	GameSoundMgr::get()->StopGameBGM();
	//}
	//if (ImGui::Button("STOP RESULT BGM")) {
	//	GameSoundMgr::get()->StopResultBGM();
	//}
	//if (ImGui::Button("CUT")) { GameSoundMgr::get()->PlayCutSE(); }
	//if (ImGui::Button("SLAP")) { GameSoundMgr::get()->PlaySlapSE(); }
	//if (ImGui::Button("BUTTON")) { GameSoundMgr::get()->PlayButtonSE(); }
	//if (ImGui::Button("CANCEL")) { GameSoundMgr::get()->PlayCancelSE(); }
	//if (ImGui::Button("PULL")) { GameSoundMgr::get()->PlayPullSE(); }


	//myImgui::EndDrawImGui();

	test_ranking.DebugDraw();


	testobj->DisplayObjectStatus(true);

	//testFBX_NoBone->DisplayObjectStatus(true);

}

void EngineDebugScene::DrawParticle()
{
	testp->Update();
	testp->Draw(particleTex);
}
