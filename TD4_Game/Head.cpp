#include "Head.h"

std::shared_ptr<fbxModel> Head::headModelStatic = std::make_shared<fbxModel>();
std::shared_ptr<fbxModel> Head::hairModelStatic = std::make_shared<fbxModel>();
std::shared_ptr<fbxModel> Head::afroModelStatic = std::make_shared<fbxModel>();

void Head::setPlayerPtr(std::shared_ptr<ProtoPlayer> ptr)
{
	//playerPtr = ptr;
}

void Head::loadHeadModel()
{
	headModelStatic.reset(FbxLoader::GetInstance()->LoadFBXFile("hage_1"));
	headModelStatic.reset(FbxLoader::GetInstance()->LoadFBXFile("ippon"));
	headModelStatic.reset(FbxLoader::GetInstance()->LoadFBXFile("kamihusahusa"));
}
