#include "FieldDrawer.h"

#include <FbxLoader.h>

void FieldDrawer::Init()
{
	wallModel = std::make_shared<fbxModel>();
	wallModel.reset(FbxLoader::GetInstance()->LoadFBXFile("wall"));
	floorModel = std::make_shared<fbxModel>();
	floorModel.reset(FbxLoader::GetInstance()->LoadFBXFile("floor"));

	for (auto& w : wallObject) {
		w.reset(SetModel_FBX(wallModel));

		w->SetAffineParam(RVector3(1, 1, 1),
			RVector3(0, 0, 0),
			RVector3(0, 0, 0));
	}

	transes[0] = RVector3(-180, 100, 0);
	transes[1] = RVector3(100, 0, 0);
	transes[2] = RVector3(180, 100, 0);
	transes[3] = RVector3(-100, 0, 0);

	for (int i = 0; i < 4; i++) {
		scales[i] = RVector3(1, 1, 1);
		rots[i] = RVector3(0, i * 90.f, 0);

		wallObject[i] = std::make_shared<Object3d>();
		wallObject[i].reset(SetModel_FBX(wallModel));

		wallObject[i]->SetAffineParam(scales[i], rots[i], transes[i]);
	}

	floorObject = std::make_shared<Object3d>();
	floorObject.reset(SetModel_FBX(floorModel));

	floorObject->SetAffineParam(RVector3(2, 1, 2),
		RVector3(0, 0, 0),
		RVector3(0, -10, 0));
}

void FieldDrawer::SetTitleMode()
{
	isgame = false;

	floorObject->SetAffineParam(RVector3(10, 1, 10),
		RVector3(0, 0, 0),
		RVector3(0, -130, 0));
}

void FieldDrawer::SetGameMode()
{
	isgame = true;

	floorObject->SetAffineParam(RVector3(10, 1, 10),
		RVector3(0, 0, 0),
		RVector3(0, -130, 0));
}


void FieldDrawer::Draw()
{

	//for (auto& w : wallObject) {
	//	if (isgame) { w->DrawObject(); }
	//}
	//if (isgame) {
	//	wallObject[0]->DrawObject();
	//	wallObject[2]->DrawObject();
	//}



	floorObject->DrawObject();
}
