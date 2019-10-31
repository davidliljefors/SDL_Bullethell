#include "Vector2D.h"
#include <cmath>

namespace FG
{
	const Vector2D Vector2D::Zero = { 0.0f, 0.0f };
	const Vector2D Vector2D::One = { 1.0f, 1.0f };
	const Vector2D Vector2D::Left = { -1.0f, 0.0f };
	const Vector2D Vector2D::Right = { 1.0f, 0.0f };
	const Vector2D Vector2D::Up = { 0.0f, 1.0f };
	const Vector2D Vector2D::Down = { 0.0f, -1.0f };

	Vector2D::Vector2D(float x, float y) : x(x), y(y)
	{}

	Vector2D::Vector2D(const Vector2D & vector) : x(vector.x), y(vector.y)
	{}

	Vector2D Vector2D::operator+(const Vector2D& other)
	{
		return Vector2D(x + other.x, y + other.y);
	}

	Vector2D Vector2D::operator-(const Vector2D& other)
	{
		return Vector2D(x - other.x, y - other.y);
	}

	Vector2D Vector2D::operator*(float scalar)
	{
		return Vector2D(x * scalar, y * scalar);
	}

	Vector2D Vector2D::operator+=(const Vector2D& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2D Vector2D::operator-=(const Vector2D& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	float Vector2D::Magnitude() const
	{
		float result = pow(x, 2) + pow(y, 2);
		return sqrt(result);
	}
}
