#pragma once

#include "NY_Object3DMgr.h"
#include "SpriteManager.h"
#include "RenderTargetManager.h"
#include "DifferrdRenderingMgr.h"
#include "ParticleManager.h"
#include "RTex.h"

//•`‰æ‡‚ğw’è‚µ‚Ä•`‰æ‚ğŒø—¦‰»
class GraphicManager
{
public:
	GraphicManager() = default;
	~GraphicManager() = default;

	void Init();

	void StartDraw();

	void StartDeferredDraw();

	void EndDefferdDraw();

	void StartForwardDraw3D();

	void StartParticleDraw3D();

	void StartParticleDraw2D();

	void StartSpriteDraw();

	void FinishDraw();

private:

	DiferredRenderingMgr m_deferredRender;

	ParticleDrawManager m_particleDrawMgr;





};

