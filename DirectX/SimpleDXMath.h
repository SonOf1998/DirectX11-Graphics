#pragma once

#include <DirectXMath.h>

using namespace DirectX;

constexpr float PI_180 = XM_PI / 180;

inline constexpr float toRad(float angle)
{
	return (angle / 180) * XM_PI;
}

template<typename T>
T&& vmax(T&& val)
{
	return std::forward<T>(val);
}

template<typename T0, typename T1, typename... Ts>
auto vmax(T0&& val1, T1&& val2, Ts&&... vs)
{
	return (val1 > val2) ?
		vmax(val1, std::forward<Ts>(vs)...) :
		vmax(val2, std::forward<Ts>(vs)...);
}


inline XMFLOAT3 operator+(const XMFLOAT3& a, const XMFLOAT3& b)
{
	XMVECTOR aVec = XMLoadFloat3(&a);
	XMVECTOR bVec = XMLoadFloat3(&b);
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, aVec + bVec);

	return ret;
}

inline XMFLOAT3 operator-(const XMFLOAT3& a, const XMFLOAT3& b)
{
	XMVECTOR aVec = XMLoadFloat3(&a);
	XMVECTOR bVec = XMLoadFloat3(&b);
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, aVec - bVec);

	return ret;
}

inline XMFLOAT3 operator*(const XMFLOAT3& a, const XMFLOAT3& b)
{
	XMVECTOR aVec = XMLoadFloat3(&a);
	XMVECTOR bVec = XMLoadFloat3(&b);
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, aVec * bVec);

	return ret;
}

inline XMFLOAT3 operator/(const XMFLOAT3& a, const XMFLOAT3& b)
{
	XMVECTOR aVec = XMLoadFloat3(&a);
	XMVECTOR bVec = XMLoadFloat3(&b);
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, aVec / bVec);

	return ret;
}

inline XMVECTOR operator*(const XMVECTOR& v, const XMMATRIX& m)
{
	return XMVector4Transform(v, m);
}
inline XMMATRIX Transpose(const XMMATRIX& m)
{
	return XMMatrixTranspose(m);
}

inline XMMATRIX Inverse(const XMMATRIX& m)
{
	return XMMatrixInverse(nullptr, m);
}

inline bool operator==(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator==(const XMVECTOR& a, const XMVECTOR& b)
{
	return XMVector3Equal(a, b);
}

inline bool operator!=(const XMFLOAT3 a, const XMFLOAT3& b)
{
	return !(a == b);
}

inline bool operator!=(const XMVECTOR& a, const XMVECTOR& b)
{
	return !(a == b);
}

inline float Dot(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline XMFLOAT3 Cross(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return XMFLOAT3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline float Length(const XMVECTOR& v)
{
	return XMVectorGetX(XMVector3Length(v));
}

inline float Length(const XMFLOAT3& v)
{
	return Length(XMLoadFloat3(&v));
}

inline float Distance(const XMFLOAT2& v1, const XMFLOAT2& v2)
{
	return Length(XMLoadFloat2(&v1) - XMLoadFloat2(&v2));
}

inline XMFLOAT2 operator+ (const XMFLOAT2& v1, const XMFLOAT2& v2)
{
	return XMFLOAT2(v1.x + v2.x, v1.y + v2.y);
}

inline XMFLOAT2 operator* (float mul, const XMFLOAT2& v)
{
	return XMFLOAT2(mul * v.x, mul * v.y);
}

/*inline XMFLOAT3 operator+ (const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return XMFLOAT3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}*/

inline XMFLOAT3 operator* (float mul, const XMFLOAT3& v)
{
	return XMFLOAT3(mul * v.x, mul * v.y, mul * v.z);
}

inline XMFLOAT3 Normalize(const XMFLOAT3& a)
{
	assert(a != XMFLOAT3(0, 0, 0));

	float length = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	return XMFLOAT3(a.x / length, a.y / length, a.z / length);
}

template<typename T>
inline XMVECTOR ToXMVECTOR(const T& xmfloat)
{
	static_assert(std::is_same_v<T, XMFLOAT2> || std::is_same_v<T, XMFLOAT3> || std::is_same_v<T, XMFLOAT4>, "ToXMVECTOR takes XMFLOATX as parameter");

	if constexpr (std::is_same_v<T, XMFLOAT2>)
	{
		return XMLoadFloat2(&T);
	}
	else if constexpr (std::is_same_v<T, XMFLOAT3>)
	{
		return XMLoadFloat3(&T);
	}
	else if constexpr (std::is_same_v<T, XMFLOAT4>)
	{
		return XMLoadFloat4(&T);
	}
}