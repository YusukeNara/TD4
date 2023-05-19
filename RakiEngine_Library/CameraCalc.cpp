#include "CameraCalc.h"

void CameraCalc::Init(RVector3 eyepos, RVector3 lookVec, RVector3 upVec,float rot)
{
	_eyepos = eyepos;
	_targetVec = lookVec;
	_upVec = upVec;

	RVector3 zeroRot;
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
}

void CameraCalc::CalcCamRotate(RVector3 rot)
{
}
