#include "DirectionalLight.h"

#include "NY_Camera.h"

DirectX::XMFLOAT3 DirectionalLight::dir = { 1.0f,-1.0f,1.0f };
DirectX::XMMATRIX DirectionalLight::lightCamera;

DirectionalLight::DirectionalLight()
{
	lightStatus = { 1.0f,1.0f,1.0f,1.0f };
	useFlag = true;
}

void DirectionalLight::SetLightDir(float x, float y, float z)
{
	dir.x = x;
	dir.y = y;
	dir.z = z;

	XMFLOAT3 target = { 0,0,0 };
	XMFLOAT3 up = { 0,1,0 };

	//ビュー行列格納
	lightCamera = XMMatrixLookAtLH(XMLoadFloat3(&dir), XMLoadFloat3(&target), XMLoadFloat3(&up));


}

void DirectionalLight::SetLightPos(RVector3 lightPos, RVector3 lightVec, RVector3 lightUp)
{
	lightCamera = XMMatrixLookAtLH(XMLoadFloat3(&lightPos), XMLoadFloat3(&lightVec), XMLoadFloat3(&lightUp));

	//座標とベクトルからライト方向を計算
	dir = { RVector3(lightVec - lightPos).norm() };

	//dir.y = -dir.y;
}

void DirectionalLight::SetLightDirection(float x, float y, float z, float lightPower)
{
	lightStatus.x = x;
	lightStatus.y = y;
	lightStatus.z = z;
	lightStatus.w = lightPower;

}

void DirectionalLight::SetLightUseFlag(bool useFlag)
{
	this->useFlag = useFlag;
}

void DirectionalLight::SetLightUseSpecular(bool isUse)
{
	this->useSpecular = isUse;
}
