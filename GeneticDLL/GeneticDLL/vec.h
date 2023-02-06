#ifndef __VEC_H__
#define __VEC_H__

#include <math.h>
#include <iostream>

struct vec3
{
	float x;
	float y;
	float z;

	vec3() {}
	vec3(float tx, float ty, float tz)
	{
		x = tx;
		y = ty;
		z = tz;
	}

	vec3 operator -() const
	{
		return vec3(-x, -y, -z);
	}

	vec3 operator +(const vec3& v)const
	{
		return vec3(x + v.x, y + v.y, z + v.z);
	}

	vec3 operator *(const float s)const
	{
		return vec3(x * s, y * s, z * s);
	}

	vec3 operator *(const vec3& v)const
	{
		return vec3(x * v.x, y * v.y, z * v.z);
	}

	vec3 operator /(const float s)const
	{
		float r = float(1.0) / s;
		return *this * r;
	}

	vec3& operator +=(const vec3& v)
	{
		x += v.x; y += v.y;z += v.z; return *this;
	}

	vec3& operator -=(const vec3& v)
	{
		x -= v.x; y -= v.y;z -= v.z; return *this;
	}
	vec3& operator *=(const float s)
	{
		x *= s; y *= s;z *= s; return *this;
	}
	vec3& operator *=(const vec3& v)
	{
		x *= v.x; y *= v.y;z *= v.z; return *this;
	}
	vec3& operator /=(const float s)
	{
		float r = float(1.0) / s;
		*this *= r;
		return *this;
	}

	
};

static float length(const vec3& v)
{
	float t = v.x * v.x + v.y * v.y + v.z * v.z;
	return std::sqrt(t);
}
static vec3 normalize(const vec3& v)
{
	return v / length(v);
}

#endif



