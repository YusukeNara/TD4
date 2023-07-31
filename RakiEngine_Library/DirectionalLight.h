#pragma once
#include "BaseLight.h"
#include <DirectXMath.h>
#include "RVector.h"

class DirectionalLight 
{
public:

	DirectionalLight();
	~DirectionalLight() = default;

	static void SetLightDir(float x, float y, float z);

	static void SetLightPos(RVector3 lightPos, RVector3 lightVec, RVector3 lightUp);

	static DirectX::XMFLOAT3 GetLightDir() { return dir; }

	static DirectX::XMMATRIX GetLightCamera() { return lightCamera; }

	void SetLightDirection(float x, float y, float z, float lightPower);

	void SetLightUseFlag(bool useFlag);

	void SetLightUseSpecular(bool isUse);

	DirectX::XMFLOAT4 GetLightDirection() { return lightStatus; }
	
	bool GetLightUseFlag() { return useFlag; }

	bool GetLightUseSpecular() { return useSpecular; }

	struct ConstBuffData
	{
		DirectX::XMFLOAT4 dir;
		unsigned int useFlag;
		unsigned int useSpecular;
		DirectX::XMFLOAT2 pad;
	};

private:


	//ÉJÉÅÉâï˚å¸
	static DirectX::XMFLOAT3 dir;

	static DirectX::XMMATRIX lightCamera;

	DirectX::XMFLOAT4 lightStatus;
	bool useFlag;
	bool useSpecular;
};

