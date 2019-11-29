#pragma once
#include <cmath>

namespace FG
{
	class Vector2D
	{
		static constexpr float PI = 3.141592f;
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
		bool operator==(const Vector2D& other);


		inline float GetAngle() const {
			return atan2(-y, x) * 180.f / PI;
		}
		inline float Magnitude() const
		{
			return std::sqrt(x * x + y * y);
		}

		Vector2D Normalized() const
		{
			float mag = Magnitude();
			return Vector2D(x / mag, y / mag);
		}

		//Returns a normalized Vector2D from an angle
		static Vector2D AngleToVector2D(float angle);
		static float Vector2DToAngle(const Vector2D& from, const Vector2D& to);
		static Vector2D GetVectorBetweenPositions(const Vector2D& from, const Vector2D& to);

		static const Vector2D Zero;
		static const Vector2D One;
		static const Vector2D Left;
		static const Vector2D Right;
		static const Vector2D Up;
		static const Vector2D Down;
	};
}
