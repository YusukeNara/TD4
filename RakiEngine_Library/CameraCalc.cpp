#include "CameraCalc.h"

void CameraCalc::Init(RVector3 eyepos, RVector3 lookVec, RVector3 upVec,float rot)
{
	_eyepos = eyepos;
	_targetVec = lookVec;
	_upVec = upVec;

	//注視点ベクトルをもとに上方向ベクトルを回転
	RQuaternion P = quaternion(_upVec.x, _upVec.y, _upVec.z, 0.f);
	RQuaternion Q = quaternion(lookVec.x * sinf(rot / 2.0f),
		lookVec.y * sinf(rot / 2.0f),
		lookVec.z * sinf(rot / 2.0f),
		cosf(rot / 2.0f));
	RQuaternion R = quaternion(-lookVec.x * sinf(rot / 2.0f),
		-lookVec.y * sinf(rot / 2.0f),
		-lookVec.z * sinf(rot / 2.0f),
		cosf(rot / 2.0f));
	RQuaternion A = R * P * Q;
	_upVec = RVector3(A.x, A.y, A.z);

	//上ベクトルをもとに注視点ベクトルを横回転
	RQuaternion P2 = quaternion(_targetVec.x, _targetVec.y, _targetVec.z, 0.f);
	RQuaternion Q2 = quaternion(_upVec.x * sinf(rot / 2.0f),
		_upVec.y * sinf(rot / 2.0f),
		_upVec.z * sinf(rot / 2.0f),
		cosf(rot / 2.0f));
	RQuaternion R2 = quaternion(-_upVec.x * sinf(rot / 2.0f),
		-_upVec.y * sinf(rot / 2.0f),
		-_upVec.z * sinf(rot / 2.0f),
		cosf(rot / 2.0f));
	RQuaternion A2 = R2 * P2 * Q2;
	_targetVec = RVector3(A2.x, A2.y, A2.z);

	//上ベクトルと原点の外積を取って右方向の軸を決定

}

void CameraCalc::CalcCamRotate(RVector3 rot)
{
	//注視点ベクトルから上ベクトル




}
