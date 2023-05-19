#pragma once
#include "Quaternion.h"


//カメラ計算クラス
//カメラ行列の機能は既存のものだが、それを計算するために必要な計算をする
class CameraCalc
{
public:
	CameraCalc(){}
	~CameraCalc(){}

	void Init(RVector3 eyepos, RVector3 lookVec, RVector3 upVec, float rot = 0.0f);

	void CalcCamRotate(RVector3 rot);




public:
	//各軸のカメラ回転角
	RVector3 _camrot;
	//視点座標
	RVector3 _eyepos;
	//注視点ベクトル
	RVector3 _targetVec;
	//上ベクトル
	RVector3 _upVec;
	//右ベクトル
	RVector3 _rightVec;


};

