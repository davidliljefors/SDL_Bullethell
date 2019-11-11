#pragma once
#include <cmath>

namespace FG
{
	class Vector2D
	{
	public:
		float x = 0.0f;
		float y = 0.0f;

		Vector2D() {}
		Vector2D(const Vector2D& other);
		Vector2D(float x, float y);

		Vector2D operator+(const Vector2D& other) const;
		Vector2D operator-(const Vector2D& other) const;
		Vector2D operator*(const Vector2D& other) const;
		Vector2D operator*(float scalar) const;
		Vector2D operator+=(const Vector2D& other);
		Vector2D operator-=(const Vector2D& other);

		inline float Magnitude()
		{
			return std::sqrt(x * x + y * y);
		}

		static const Vector2D Zero;
		static const Vector2D One;
		static const Vector2D Left;
		static const Vector2D Right;
		static const Vector2D Up;
		static const Vector2D Down;
	};
}
