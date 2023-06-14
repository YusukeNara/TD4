#include "Head.h"

//std::weak_ptr<ProtoPlayer> Head::playerPtr = std::make_shared<ProtoPlayer>();
std::shared_ptr<Object3d> Head::headObject = std::make_shared<Object3d>();
std::shared_ptr<Object3d> Head::hairObject = std::make_shared<Object3d>();
std::shared_ptr<Object3d> Head::afroObject = std::make_shared<Object3d>();

void Head::setPlayerPtr(std::shared_ptr<ProtoPlayer> ptr)
{
	//playerPtr = ptr;
}

void Head::loadHeadModel()
{
	headObject.reset(LoadModel_FBXFile("hage_1"));
	hairObject.reset(LoadModel_FBXFile("ippon"));
	afroObject.reset(LoadModel_FBXFile("kamihusahusa"));
}
