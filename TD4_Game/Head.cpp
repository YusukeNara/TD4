#include "Head.h"

void Head::setPlayerPtr(std::shared_ptr<ProtoPlayer> ptr)
{
	//playerPtr = ptr;
}

void Head::loadHeadModel()
{
	headModelStatic = std::make_shared<fbxModel>();
	hairModelStatic = std::make_shared<fbxModel>();
	afroModelStatic = std::make_shared<fbxModel>();

	headModelStatic.reset(FbxLoader::GetInstance()->LoadFBXFile("hage_1"));
	headModelStatic.reset(FbxLoader::GetInstance()->LoadFBXFile("ippon"));
	headModelStatic.reset(FbxLoader::GetInstance()->LoadFBXFile("kamihusahusa"));
}
