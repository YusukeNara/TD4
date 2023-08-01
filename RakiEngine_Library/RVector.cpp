#include "RVector.h"

#include "Raki_WinAPI.h"
#include "NY_Camera.h"
#include "FPS.h"

#include <iostream>
#include <array>

bool RV3Colider::ColisionSphereToPlane(const Sphere &sphere, const Plane &plane, RVector3 *coliPos)
{
	//平面と球の中心との距離を求める
	float dist = dot(sphere.center, plane.normal) - plane.distance;
	//距離の絶対値が半径より大きければあたってない
	if (fabsf(dist) > sphere.rad) { return false; }
	//あたっている場合は疑似交点を計算
	*coliPos = -dist * plane.normal + sphere.center;
	//衝突
	return true;
}

RVector3 RV3Colider::CalcScreen2World(const XMFLOAT2& scrPos, float fz)
{
	XMVECTOR pos;
	//射影変換行列とビューポート行列の逆行列を格納する変数
	XMMATRIX InvPrj, InvVP, InvV;
	//各行列の逆行列を出す
	InvPrj = XMMatrixInverse(nullptr, camera->GetMatrixProjection());
	//ビューポート行列はもとはない？のでここで定義して逆行列を出す
	InvVP = XMMatrixIdentity();
	InvVP.r[0].m128_f32[0] = Raki_WinAPI::window_width / 2.0f;
	InvVP.r[1].m128_f32[1] = -Raki_WinAPI::window_height / 2.0f;
	InvVP.r[3].m128_f32[0] = Raki_WinAPI::window_width / 2.0f;
	InvVP.r[3].m128_f32[1] = Raki_WinAPI::window_height / 2.0f;
	InvVP = XMMatrixInverse(nullptr, InvVP);

	InvV = XMMatrixInverse(nullptr, camera->GetMatrixView());

	XMMATRIX inverce = InvVP * InvPrj * InvV;
	XMVECTOR scr = { scrPos.x,scrPos.y,fz };

	pos = XMVector3TransformCoord(scr, inverce);

	RVector3 returnpos = { pos.m128_f32[0],pos.m128_f32[1],pos.m128_f32[2] };
	return returnpos;
}

bool RV3Colider::ColisionRayToAABB(Ray& ray, const Rv3AABB& box, float* distance, RVector3* colisionPos)
{
	//xyzそれぞれについてスラブの判定を取る

	float t = -FLT_MAX;
	float t_max = FLT_MAX;

	std::array<float, 3> p = {}, d = {}, min = {}, max = {};
	memcpy(p.data(), &ray.start, sizeof(RVector3));
	memcpy(d.data(), &ray.dir, sizeof(RVector3));
	memcpy(min.data(), &box.min, sizeof(RVector3));
	memcpy(max.data(), &box.max, sizeof(RVector3));
	for (int i = 0; i < 3; i++) {
		if (abs(d[i]) < FLT_EPSILON) {
			if (p[i] < min[i] || p[i] > max[i])
				return false; // 交差していない
		}
		else {
			// スラブとの距離を算出
			// t1が近スラブ、t2が遠スラブとの距離
			float odd = 1.0f / d[i];
			float t1 = (min[i] - p[i]) * odd;
			float t2 = (max[i] - p[i]) * odd;
			if (t1 > t2) {
				float tmp = t1; t1 = t2; t2 = tmp;
			}

			if (t1 > t) t = t1;
			if (t2 < t_max) t_max = t2;

			// スラブ交差チェック
			if (t >= t_max)
				return false;
		}
	}

	// 交差している
	if (colisionPos) {
		*colisionPos = ray.start + t * (ray.dir);
	}

	return true;
}

const RVector3 Rv3Ease::lerp(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - t) + end * t;
}

const RVector3 Rv3Ease::InQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - t * t) + end * (float)pow(t, 2);
}

const RVector3 Rv3Ease::OutQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - (1.0f - (1.0f - t) * (1.0f - t))) + end * (1.0f - (1.0f - t) * (1.0f - t));
}

const RVector3 Rv3Ease::InOutQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return t < 0.5f ? (start * (1.0f - t)) + end * (t * t) : start * (1.0f - t) + end * (1 - (float)pow(-2 * t + 2, 2) / 2);
}

const RVector3 Rv3Ease::InCubic(const RVector3& s, const RVector3& e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - (t * t * t)) + end * (t * t * t);
}

const RVector3 Rv3Ease::OutCubic(const RVector3& s, const RVector3& e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - (1.0f - (float)pow(1.0f - t, 3))) + end * (1.0f - (float)pow(1.0f - t, 3));
}

const RVector3 Rv3Ease::InOutCubic(const RVector3& s, const RVector3& e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return t < 0.5 ? (start * (1.0f - (4.0f * t * t * t)) + end * (4.0f * t * t * t)) : 
		start * (1.0f - (1.0f - float(pow(-2.0f * t + 2.0f, 3) / 2.0f))) + end * (1.0f - float(pow(-2.0f * t + 2.0f, 3) / 2.0f));
}

float CalcVelocityToForceAndMass(const float force, const float mass, float nowVel, float &acc)
{
	float accel = acc + CalcAccelToForceAndMass(force, mass);

	float vel = nowVel + accel;

	acc = accel;

	return vel;
}

float CalcAccelToForceAndMass(const float force, const float mass)
{
	return force / mass;
}

float CalcAccelToFramePerVel(float onesecmove)
{
	return onesecmove / 60.0f;
}

float CalcGravity(float start, float end, float v0, int time,float &v1)
{


	//現在の速度^2 = 初速度^2 + -2*重力加速度
	float v = (v0 * v0) + (-2 * CalcAccelToFramePerVel(9.8f));

	float t1 = time / 60.0f;
	float t2 = (time - 1) / 60.0f;

	//現在座標 = 開始点 + 現在速度
	float now = start + (sqrtf(v) * -t2);

	float next = now + (sqrtf(v) * -t1);

	if (next < end) {//下降中、終了点より↓
		now = end;//値を丸める
	}

	//結果
	return next;
}

void Rv3Ease::Rv3Easing::Init(RV3_EASE_TYPE type, RVector3 startPos, RVector3 endPos, int playFrame)
{
	this->startPos	=	startPos;
	this->endPos	=	endPos;
	this->playFrame =	playFrame;
	this->type		=	type;
	this->resultPos =	startPos;
}

void Rv3Ease::Rv3Easing::Play()
{
	if (!isplay) { isplay = true; }
}

RVector3 Rv3Ease::Rv3Easing::Update()
{
	if (!isplay) { return startPos; }

	if (frame >= playFrame) { 
		isEnded = true;
		return endPos; 
	}

	frame++;

	float rate = float(frame) / float(playFrame);

	//各種イージング計算
	switch (type)
	{
	case Rv3Ease::RV3_EASE_TYPE::EASE_LERP:
		resultPos = Rv3Ease::lerp(startPos, endPos, rate);
		break;
	case Rv3Ease::RV3_EASE_TYPE::EASE_QUAD_IN:
		resultPos = Rv3Ease::InQuad(startPos, endPos, rate);
		break;
	case Rv3Ease::RV3_EASE_TYPE::EASE_QUAD_OUT:
		resultPos = Rv3Ease::OutQuad(startPos, endPos, rate);
		break;
	case Rv3Ease::RV3_EASE_TYPE::EASE_QUAD_INOUT:
		resultPos = Rv3Ease::InOutQuad(startPos, endPos, rate);
		break;
	case Rv3Ease::RV3_EASE_TYPE::EASE_CUBIC_IN:
		resultPos = Rv3Ease::InCubic(startPos, endPos, rate);
		break;
	case RV3_EASE_TYPE::EASE_CUBIC_OUT:
		resultPos = Rv3Ease::OutCubic(startPos, endPos, rate);
		break;
	case RV3_EASE_TYPE::EASE_CUBIC_INOUT:
		resultPos = Rv3Ease::InOutCubic(startPos, endPos, rate);
		break;
	default:
		break;
	}
	return resultPos;
}

void Rv3Ease::Rv3Easing::Reset()
{
	frame = 0;
	isplay = false;
	resultPos = startPos;
	isEnded = false;
}

void Rv3Ease::Rv3Spline::Init(RVector3* array, int pCount, int playFrame)
{
	//制御点が足りない場合
	if (pCount < 4) { return; }

	for (int i = 0; i < pCount; i++) {
		splinePoints.push_back(array[i]);
	}

	this->playFrame = playFrame;
}

void Rv3Ease::Rv3Spline::Play()
{
	isPlay = true;
}

RVector3 Rv3Ease::Rv3Spline::Update()
{
	if (!isPlay) { return resultPos; }
	if (frame >= playFrame) { return resultPos; }

	frame++;

	//制御点の総数に応じて、再生フレームを分割
	int pFramePerCurve = playFrame / int(splinePoints.size() - 3);
	if ((frame - (pFramePerCurve * nowPlayIndex)) > pFramePerCurve) {
		nowPlayIndex++;
	}

	float rate = float(frame - (pFramePerCurve * nowPlayIndex)) / float(pFramePerCurve);

	RVector3 &p0 = splinePoints[nowPlayIndex];
	RVector3 &p1 = splinePoints[nowPlayIndex + 1];
	RVector3 &p2 = splinePoints[nowPlayIndex + 2];
	RVector3 &p3 = splinePoints[nowPlayIndex + 3];

	resultPos = SplineCurve4(p0, p1, p2, p3, rate);

	return resultPos;
}

void Rv3Ease::Rv3Spline::Reset()
{
	resultPos = splinePoints[1];
	frame = 0;
	nowPlayIndex = 0;
	isPlay = false;
}

RVector3 Rv3Ease::Rv3Spline::SplineCurve4(const RVector3& p0, const RVector3& p1, const RVector3& p2, RVector3& p3, float t)
{
	RVector3 result = 0.5 * (((2 * p1) + (-p0 + p2) * t) +
		(((2 * p0) - (5 * p1) + (4 * p2) - (p3)) * float(pow(t, 2))) +
		(((-p0) + (3 * p1) - (3 * p2) + (p3)) * float(pow(t, 3))));


	return result;
}

void Rv3Ease::Rv3Bezier3::Init(RVector3 start, RVector3 end, RVector3 ctrlPoint, int playFrame, RV3_EASE_TYPE type)
{
	ease1.Init(type, start, ctrlPoint, playFrame);
	ease2.Init(type, ctrlPoint, end, playFrame);
	bezier.Init(type, ease1.Update(), ease2.Update(), playFrame);

	resultPos = start;
	frame = 0;
	this->playFrame = playFrame;
	isPlay = false;
}

void Rv3Ease::Rv3Bezier3::Play()
{
	isPlay = true;
	ease1.Play();
	ease2.Play();
	bezier.Play();
}

RVector3 Rv3Ease::Rv3Bezier3::Update()
{
	if (!isPlay) { return resultPos; }
	if (frame >= playFrame) { return resultPos; }

	frame++;

	bezier.SetStartPos(ease1.Update());
	bezier.SetEndPos(ease2.Update());

	resultPos = bezier.Update();

	return 	resultPos;
}

void Rv3Ease::Rv3Bezier3::Reset()
{
	frame = 0;
	isPlay = false;
	ease1.Reset();
	ease2.Reset();
	bezier.Reset();
	resultPos = ease1.GetStart();
}