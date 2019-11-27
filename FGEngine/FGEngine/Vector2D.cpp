#include "Vector2D.h"

constexpr float PI = 3.141592f;

namespace FG
{
	const Vector2D Vector2D::Zero = { 0.0f, 0.0f };
	const Vector2D Vector2D::One = { 1.0f, 1.0f };
	const Vector2D Vector2D::Left = { -1.0f, 0.0f };
	const Vector2D Vector2D::Right = { 1.0f, 0.0f };
	const Vector2D Vector2D::Up = { 0.0f, 1.0f };
	const Vector2D Vector2D::Down = { 0.0f, -1.0f };

	Vector2D::Vector2D(const Vector2D& other) : x(other.x), y(other.y)
	{ }

	Vector2D::Vector2D(float x, float y) : x(x), y(y)
	{}

	Vector2D Vector2D::operator+(const Vector2D & other) const
	{
		return Vector2D(x + other.x, y + other.y);
	}

	Vector2D Vector2D::operator-(const Vector2D& other) const
	{
		return Vector2D(x - other.x, y - other.y);
	}

	Vector2D Vector2D::operator*(const Vector2D& other) const
	{
		return Vector2D(x * other.x, y * other.y);
	}

	Vector2D Vector2D::operator*(float scalar) const
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

	bool Vector2D::operator==(const Vector2D& other)
	{
		return x == other.x && y == other.y;
	}

	Vector2D Vector2D::AngleToVector2D(float angle)
	{
		return Vector2D(cosf(angle * (PI / 180.0f)), sinf(angle * (PI / 180.0f))) ;
	}

	float Vector2D::Vector2DToAngle(const Vector2D& from, const Vector2D& to)
	{
		return acos((from.x * from.y + to.x * to.y) / (from.Magnitude() * to.Magnitude())) * 180.0f / PI;
	}
}
