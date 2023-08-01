#include "Head.h"

std::shared_ptr<fbxModel> Head::headModelStatic = std::make_shared<fbxModel>();
std::shared_ptr<fbxModel> Head::hairModelStatic = std::make_shared<fbxModel>();
std::shared_ptr<fbxModel> Head::afroModelStatic = std::make_shared<fbxModel>();

UINT Head::slapTex = 0;
UINT Head::pullTex = 0;

void Head::setPlayerPtr(std::shared_ptr<ProtoPlayer> ptr)
{
	//playerPtr = ptr;
}

void Head::loadHeadModel()
{
	headModelStatic.reset(FbxLoader::GetInstance()->LoadFBXFile("hage_1"));
	hairModelStatic.reset(FbxLoader::GetInstance()->LoadFBXFile("ippon"));
	afroModelStatic.reset(FbxLoader::GetInstance()->LoadFBXFile("kamihusahusa"));
}

void Head::setStaticData()
{
	slapTex = TexManager::LoadTexture("Resources/white1x1.png");
	pullTex = TexManager::LoadTexture("Resources/SingleHair.png");
}
