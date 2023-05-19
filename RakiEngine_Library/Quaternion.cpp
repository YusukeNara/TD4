#include "Quaternion.h"

RQuaternion quaternion(float x, float y, float z, float w)
{
	RQuaternion result{ x,y,z,w };
	return result;
}

RQuaternion quaternion(RVector3 &v, float angle)
{
	float _sin = static_cast<float>(sin(angle / 2.0f));
	return quaternion(_sin * v.x, _sin * v.y, _sin * v.z, static_cast<float>(cos(angle / 2.0f)));
}

RQuaternion quaternion(const RVector3 &v, const RQuaternion &q)
{
	RQuaternion qq = conjugate(q);
	RQuaternion qv = quaternion(v.x, v.y, v.z, 0);

	return q * qv * qq;
}

float dot(const RQuaternion &q1, const RQuaternion &q2)
{
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}

float lenght(const RQuaternion &q)
{
	return (float)sqrt(dot(q, q));
}

RQuaternion normalize(const RQuaternion &q)
{
	RQuaternion result = q;
	float len = lenght(result);
	if (len != 0) {
		result /= len;
	}
	return result;
}

RQuaternion conjugate(const RQuaternion &q)
{
	//‹••”‚ð-‚É‚µ‚Ä•Ô‹p
	return quaternion(q.x + -q.x * 2, q.y + -q.y * 2, q.z + -q.z * 2, q.w);
}

RQuaternion operator+(const RQuaternion &q)
{
	return q;
}

RQuaternion operator-(const RQuaternion &q)
{
	RQuaternion result = { -q.x,-q.y,-q.z,-q.w };
	return result;
}

RQuaternion &operator+=(RQuaternion &q1, const RQuaternion &q2)
{
	q1.x += q2.x;
	q1.y += q2.y;
	q1.z += q2.z;
	q1.w += q2.w;
	return q1;
}

RQuaternion &operator-=(RQuaternion &q1, const RQuaternion &q2)
{
	q1.x -= q2.x;
	q1.y -= q2.y;
	q1.z -= q2.z;
	q1.w -= q2.w;
	return q1;
}

RQuaternion &operator*=(RQuaternion &q, float s)
{
	q.x *= s;
	q.y *= s;
	q.z *= s;
	q.w *= s;
	return q;
}

RQuaternion &operator/=(RQuaternion &q, float s)
{
	return q *= 1.0f / s;
}

RQuaternion &operator*=(RQuaternion &q1, const RQuaternion &q2)
{
	RQuaternion result{
		 (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y) + (q1.w * q2.x),//x
		-(q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x) + (q1.w * q2.y),//y
		 (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w) + (q1.w * q2.z),//z
		-(q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z) + (q1.w * q2.w) //w
	};
	q1 = result;
	return q1;
}

RQuaternion &operator/=(RQuaternion &q1, const RQuaternion &q2)
{
	return q1;
}

RQuaternion operator+(const RQuaternion &q1, const RQuaternion &q2)
{
	RQuaternion result = q1;
	return result += q2;
}

RQuaternion operator-(const RQuaternion &q1, const RQuaternion &q2)
{
	RQuaternion result = q1;
	return result -= q2;
}

RQuaternion operator*(const RQuaternion &q1, const RQuaternion &q2)
{
	RQuaternion result = q1;
	return result *= q2;
}

RQuaternion operator*(const RQuaternion &q1, float s)
{
	RQuaternion result = q1;
	return result *= s;
}

RQuaternion operator*(float s, const RQuaternion &q1)
{
	RQuaternion result = q1;
	return result *= s;
}

RQuaternion operator/(const RQuaternion &q1, float s)
{
	RQuaternion result = q1;
	return result /= s;
}

RQuaternion slarp(const RQuaternion &q1, const RQuaternion &q2, float t)
{
	float cos = dot(q1, q2);
	RQuaternion t2 = q2;
	if (cos < 0.0f) {
		cos = -cos;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	if ((1.0f - cos) > 0.001f) {
		float theta = (float)acos(cos);
		k0 = (float)(sin(theta * k0) / sin(theta));
		k1 = (float)(sin(theta * k1) / sin(theta));
	}
	return q1 * k0 + t2 * k1;
}

RQuaternion larp(const RQuaternion &q1, const RQuaternion &q2, float t)
{
	float cos = dot(q1, q2);
	RQuaternion t2 = q2;
	if (cos < 0.0f) {
		cos = -cos;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = 1.0f;
	return q1 * k0 + t2 * k1;
}

DirectX::XMMATRIX rotate(const RQuaternion &q)
{
	float xx = q.x * q.x * 2.0f;
	float yy = q.y * q.y * 2.0f;
	float zz = q.z * q.z * 2.0f;
	float xy = q.x * q.y * 2.0f;
	float xz = q.x * q.z * 2.0f;
	float yz = q.y * q.z * 2.0f;
	float wx = q.w * q.x * 2.0f;
	float wy = q.w * q.y * 2.0f;
	float wz = q.w * q.z * 2.0f;

	DirectX::XMMATRIX result = {
		1.0f - yy - zz,xy + wz      ,xz - wy      ,0.0f,
		xy - wz       ,1.0f - xx - zz,yz + wx      ,0.0f,
		xz + wy       ,yz - wx      ,1.0f - xx - yy,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

RQuaternion quaternion(const DirectX::XMMATRIX &m)
{
	RQuaternion result;
	float tr = m.r[0].m128_f32[0] + m.r[1].m128_f32[1] + m.r[2].m128_f32[2] + m.r[3].m128_f32[3];

	if (tr >= 1.0f) {
		float fourD = 2.0f * static_cast<float>(sqrt(tr));
		result.x = (m.r[1].m128_f32[2] - m.r[2].m128_f32[1]) / fourD;
		result.y = (m.r[2].m128_f32[0] - m.r[0].m128_f32[2]) / fourD;
		result.z = (m.r[0].m128_f32[1] - m.r[1].m128_f32[0]) / fourD;
		result.w = fourD / 4.0f;
		return result;
	}

	int i = 0;
	if (m.r[0].m128_f32[0] <= m.r[1].m128_f32[1]) {
		i = 1;
	}
	if (m.r[2].m128_f32[2] > m.r[i].m128_f32[i]) {
		i = 2;
	}
	int j = (i + 1) % 3;
	int k = (j + 1) % 3;
	tr = m.r[i].m128_f32[i] - m.r[j].m128_f32[j] - m.r[k].m128_f32[k] + 1.0f;
	float fourD = 2.0f * static_cast<float>(sqrt(tr));
	float qa[4];
	qa[i] = fourD / 4.0f;
	qa[j] = (m.r[j].m128_f32[i] + m.r[i].m128_f32[j]) / fourD;
	qa[k] = (m.r[k].m128_f32[i] + m.r[i].m128_f32[k]) / fourD;
	qa[3] = (m.r[j].m128_f32[k] - m.r[k].m128_f32[j]) / fourD;
	result.x = qa[0];
	result.y = qa[1];
	result.z = qa[2];
	result.w = qa[3];

	return result;
}

RVector3 getAxis(const RQuaternion &q)
{
	RVector3 result;
	float x = q.x;
	float y = q.y;
	float z = q.z;
	float _len = lenght(q);

	result.x = x / _len;
	result.y = y / _len;
	result.z = z / _len;

	return result;
}
