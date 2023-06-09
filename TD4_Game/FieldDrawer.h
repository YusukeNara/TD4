#pragma once
#include <NY_Object3DMgr.h>
#include <array>
#include <memory>

class FieldDrawer
{
public:

	static FieldDrawer* get() {
		static FieldDrawer ins;
		return &ins;
	}

	void Init();

	void SetTitleMode();

	void SetGameMode();

	void Draw();

	void Control();

private:


	std::shared_ptr<Object3d> floorObject;
	std::array<std::shared_ptr<Object3d>,4> wallObject;

	std::shared_ptr<fbxModel> wallModel;
	std::shared_ptr<fbxModel> floorModel;

	std::array<RVector3, 4> scales;
	std::array<RVector3, 4> rots;
	std::array<RVector3, 4> transes;

	bool isgame = false;
};

