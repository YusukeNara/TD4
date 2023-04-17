#pragma once
#include "BaseLight.h"
#include <DirectXMath.h>
#include "RVector.h"

class DirectionalLight 
{
public:

	static void SetLightDir(float x, float y, float z);

	static void SetLightPos(RVector3 lightPos, RVector3 lightVec, RVector3 lightUp);

	static DirectX::XMFLOAT3 GetLightDir() { return dir; }

	static DirectX::XMMATRIX GetLightCamera() { return lightCamera; }

private:
	//ƒJƒƒ‰•ûŒü
	static DirectX::XMFLOAT3 dir;

	static DirectX::XMMATRIX lightCamera;
};

