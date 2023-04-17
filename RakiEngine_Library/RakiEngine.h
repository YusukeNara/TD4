#pragma once

#include <memory>

//全機能一括管理
#include "Raki_DX12B.h"
#include "Raki_Input.h"
#include "Raki_WinAPI.h"
#include "FPS.h"
#include "NY_Object3DMgr.h"
#include "SpriteManager.h"
#include "Audio.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"
#include "RenderTargetManager.h"

class RakiEngine final
{
private:


public:
	//コンストラクタ、デストラクタ





private:
	//各クラスのインスタンス

	//windowsAPI管理部

	//DirectX12管理部
	
	//レンダーターゲット管理部
	std::unique_ptr<RenderTargetManager> renderTagtetMgr;
	//FPS管理部
	
	//テクスチャ管理部

	//Object3d管理部

	//2DSprite管理部

	//imgui管理部




};