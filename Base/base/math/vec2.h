#pragma once

#include "flops.h"


namespace base
{
	union vec2
	{
		float v[2];
		struct { float x, y; };

		float &operator[](size_t idx) { return v[idx]; }
		float operator[](size_t idx) const { return v[idx]; }

		vec2 &operator+=(const vec2 &o) { return *this = vec2{ x + o.x, y + o.y }; }

		vec2 &operator-=(const vec2 &o) { return *this = vec2{ x - o.x, y - o.y }; }
		vec2 &operator*=(const float s) { return *this = vec2{ x * s, y * s }; }
		vec2 &operator/=(const float s) { return *this = vec2{ x / s, y / s }; }

		vec2 operator-()			const { return vec2{ -x,-y }; }
		float angle()				const { return atan2f(y, x);  }
		float magnitude()			const { return sqrt(x*x + y*y); }
		vec2 normal()			    const { auto m = magnitude();  return{ x / m, y / m }; }
		vec2 perp(bool left = true) const { return left ? vec2{ -y,x } : vec2{ y,-x }; }

		vec2() : x(0), y(0) {}
		vec2(float a) :x(a), y(a) {}
		vec2(float a, float b) : x(a),y(b) {}
		vec2(float a[]) :x(a[0]), y(a[1]) {}

		static vec2 left()  { return vec2{-1, 0 }; }
		static vec2 up()    { return vec2{ 0, 1 }; }
		static vec2 right() { return vec2{ 1, 0 }; }
		static vec2 down()  { return vec2{ 0,-1 }; }

		static vec2 fromAngle(float a, float m = 1) { return vec2{cosf(a), sinf(a)} *= m; }
	};



	inline vec2 operator+(vec2 a, const vec2 &o)  { return a += o; }
	inline vec2 operator-(vec2 a, const vec2 &o)  { return a -= o; }
	inline vec2 operator*(vec2 a, float s)		   { return a *= s; }
	inline vec2 operator/(vec2 a, float s)		   { return a /= s; }

	inline vec2 operator*(float s, vec2 a)		   { return a *= s; }


	inline float dot(const vec2 &a, const vec2 &b) { return a.x*b.x + a.y*b.y; }

inline float cross(const vec2 &a, const vec2 &b, bool left = true) { return dot(a.perp(left), b); }

inline float dist(const vec2 &a, const vec2 &b) { return (a - b).magnitude(); }

inline vec2 project(const vec2 &a, const vec2 &b) { auto d = b.normal(); return d * dot(a, d); }
inline vec2 reflect(const vec2 &a, const vec2 &b) { return 2 * project(a, b) - a; }

inline vec2 min(const vec2 &a, const vec2 &b) { return vec2{ flops::min(a.x,b.x), flops::min(a.y,b.y) }; }
inline vec2 max(const vec2 &a, const vec2 &b) { return vec2{ flops::max(a.x,b.x), flops::max(a.y,b.y) }; }
inline vec2 clamp(const vec2 &v, const vec2& n, const vec2 &x) { return min(x, max(v,n)); }
inline vec2 snap(const vec2 &v, const vec2& n, const vec2 &x) { return vec2{ flops::snap(v.x,n.x,x.x),flops::snap(v.y,n.y,x.y)}; }
inline vec2 lerp(const vec2 &s, const vec2 &e, float t) { return vec2{ flops::lerp(s.x,e.x,t), flops::lerp(s.y,e.y,t) };  }
inline vec2 abs(const vec2 &a)
{
	vec2 out = a;
	if (out.x < 0) out.x *= -1;
	if (out.y < 0) out.y *= -1;

	return out;
}


inline bool operator==(const vec2 &a, const vec2 &b) { return flops::fequals(a.x, b.x) && flops::fequals(a.y, b.y); }
inline bool operator!=(const vec2 &a, const vec2 &b) { return !(a == b); }

inline bool operator>(const vec2 &a, const vec2 &b) { return (a.x > b.x && a.y > b.y); }
inline bool operator<(const vec2 &a, const vec2 &b) { return (a.x < b.x && a.y < b.y); }

}