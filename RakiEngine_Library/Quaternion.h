#pragma once
#include "RVector.h"
#include <DirectXMath.h>

struct RQuaternion {
	float x;
	float y;
	float z;
	float w;
};

//クオータニオンを作成
RQuaternion quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
//任意軸の回転から
RQuaternion quaternion(RVector3 &v, float angle);
//ベクトルvを姿勢を表すクオータニオンqで回転
RQuaternion quaternion(const RVector3 &v, const RQuaternion &q);

//内積を求める
float dot(const RQuaternion &q1, const RQuaternion &q2);

//ノルムを求める
float lenght(const RQuaternion &q);

//正規化する
RQuaternion normalize(const RQuaternion &q);

//共役四元数を求める
RQuaternion conjugate(const RQuaternion &q);

//単項演算子オーバーロード
RQuaternion operator+(const RQuaternion &q);
RQuaternion operator-(const RQuaternion &q);

//代入演算子オーバロード
RQuaternion &operator+=(RQuaternion &q1, const RQuaternion &q2);
RQuaternion &operator-=(RQuaternion &q1, const RQuaternion &q2);
RQuaternion &operator*=(RQuaternion &q, float s);
RQuaternion &operator/=(RQuaternion &q, float s);
RQuaternion &operator*=(RQuaternion &q1, const RQuaternion &q2);
RQuaternion &operator/=(RQuaternion &q1, const RQuaternion &q2);

//二項演算子オーバーロード
RQuaternion operator+(const RQuaternion &q1, const RQuaternion &q2);
RQuaternion operator-(const RQuaternion &q1, const RQuaternion &q2);
RQuaternion operator*(const RQuaternion &q1, const RQuaternion &q2);
RQuaternion operator*(const RQuaternion &q1, float s);
RQuaternion operator*(float s, const RQuaternion &q1);
RQuaternion operator/(const RQuaternion &q1, float s);

RQuaternion slarp(const RQuaternion &q1, const RQuaternion &q2, float t);
RQuaternion larp(const RQuaternion &q1, const RQuaternion &q2, float t);
DirectX::XMMATRIX rotate(const RQuaternion &q);
RQuaternion quaternion(const DirectX::XMMATRIX &m);
RVector3 getAxis(const RQuaternion &q);