#pragma once

#include "includes.h"

template<typename T>
class Vector2 {
  public:
	T x = 0.0f;
	T y = 0.0f;

	Vector2<T>() = default;
	Vector2<T>(T x, T y): x(x), y(y) {}

	Vector2<T> operator+(const Vector2<T>& other) const { return Vector2<T>(x + other.x, y + other.y); }
	Vector2<T> operator-(const Vector2<T>& other) const { return Vector2<T>(x - other.x, y - other.y); }
	Vector2<T> operator*(const Vector2<T>& other) const { return Vector2<T>(x * other.x, y * other.y); }
	Vector2<T> operator/(const Vector2<T>& other) const { return Vector2<T>(x / other.x, y / other.y); }

	Vector2<T> normalized() const {
		T mag = magnitude();
		if (mag == 0.0f) return zero();
		return Vector2<T>(x / mag, y / mag);
	}

	T dot(const Vector2<T>& other) const { return x * other.x + y * other.y; }
	T cross(const Vector2<T>& other) const { return x * other.y - y * other.x; }

	Vector2<T> lerp(const Vector2<T>& other, float t) const {
		return Vector2<T>(x + t * (other.x - x), y + t * (other.y - y));
	}
	Vector2<T> slerp(const Vector2<T>& other, float t) const {
		float dot           = normalized().dot(other.normalized());
		dot                 = std::clamp(dot, -1.0f, 1.0f);
		float      theta    = std::acos(dot) * t;
		Vector2<T> relative = other - (*this) * dot;
		relative            = relative.normalized();
		return ((*this) * std::cos(theta)) + (relative * std::sin(theta));
	}

	Vector2<T> smooth_damp(const Vector2<T>& target,
	                       const Vector2<T>& current_velocity,
	                       float             smooth_time,
	                       float             max_speed) const {
		float t           = 2.0f / smooth_time;
		t                 = std::clamp(t, 0.0f, 1.0f);
		float      omega  = 2.0f / smooth_time;
		float      x      = omega * t;
		float      exp    = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
		Vector2<T> change = target - (*this);
		Vector2<T> temp   = (current_velocity + change * omega) * t;
		Vector2<T> result = (*this) + (change * exp);
		if (result.magnitude() > max_speed) {
			result = result.normalized() * max_speed;
		}
		return result;
	}

	T magnitude() const { return std::sqrt(x * x + y * y); }

	static Vector2<T> zero() { return Vector2<T>(0, 0); }
	static Vector2<T> one() { return Vector2<T>(1, 1); }
	static Vector2<T> up() { return Vector2<T>(0, 1); }
	static Vector2<T> down() { return Vector2<T>(0, -1); }
	static Vector2<T> left() { return Vector2<T>(-1, 0); }
	static Vector2<T> right() { return Vector2<T>(1, 0); }
};

typedef Vector2<int>          Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float>        Vector2f;
typedef Vector2<double>       Vector2d;