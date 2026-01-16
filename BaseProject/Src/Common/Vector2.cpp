#include "Vector2.h"

// コンストラクタ
Vector2::Vector2(void)
{
	x = 0;
	y = 0;
}

// コンストラクタ
Vector2::Vector2(int vX, int vY)
{
	x = vX;
	y = vY;
}

Vector2::~Vector2(void)
{
}

void Vector2::operator=(const int value)
{
	x = value; y = value;
}

Vector2 Vector2::operator+(const Vector2 value)const
{
	return Vector2(x + value.x, y + value.y);
}

void Vector2::operator+=(const Vector2 value)
{
	x = x + value.x;
	y = y + value.y;
}

Vector2 Vector2::operator+(const int value) const
{
	return Vector2(x + value, y + value);
}

void Vector2::operator+=(const int value)
{
	x += value; y += value;
}

Vector2 Vector2::operator-(const Vector2 value)const
{
	return Vector2(x - value.x, y - value.y);
}

void Vector2::operator-=(const Vector2 value)
{
	x = x - value.x;
	y = y - value.y;
}

Vector2 Vector2::operator-(const int value) const
{
	return Vector2(x - value, y - value);
}

void Vector2::operator-=(const int value)
{
	x -= value; y -= value;
}

Vector2 Vector2::operator*(const Vector2 value) const
{
	return Vector2(x * value.x, y * value.y);
}

void Vector2::operator*=(const Vector2 value)
{
	x *= value.x; y *= value.y;
}

Vector2 Vector2::operator*(const int value)const
{
	return Vector2(x * value, y * value);
}

void Vector2::operator*=(const int value)
{
	x = x * value;
	y = y * value;
}

Vector2 Vector2::operator/(const Vector2 value) const
{
	return Vector2(x / value.x, y / value.y);
}

void Vector2::operator/=(const Vector2 value)
{
	x /= value.x; y /= value.y;
}

Vector2 Vector2::operator/(const int value)const
{
	return Vector2(x / value, y / value);
}

void Vector2::operator/=(const int value)
{
	x = x / value;
	y = y / value;
}

Vector2F::Vector2F(void)
{
	x = 0.0f;
	y = 0.0f;
}

Vector2F::Vector2F(float vX, float vY)
{
	x = vX;
	y = vY;
}

Vector2F::~Vector2F(void)
{
}

Vector2F Vector2F::operator+(const Vector2F value) const
{
	return Vector2F(x + value.x, y + value.y);
}

void Vector2F::operator+=(const Vector2F value)
{
	x = x + value.x;
	y = y + value.y;
}

Vector2F Vector2F::operator-(const Vector2F value) const
{
	return Vector2F(x - value.x, y - value.y);
}

void Vector2F::operator-=(const Vector2F value)
{
	x = x - value.x;
	y = y - value.y;
}

Vector2F Vector2F::operator*(const float value) const
{
	return Vector2F(x * value, y * value);
}

void Vector2F::operator*=(const float value)
{
	x = x * value;
	y = y * value;
}

Vector2F Vector2F::operator/(const float value) const
{
	return Vector2F(x / value, y / value);
}

void Vector2F::operator/=(const float value)
{
	x = x / value;
	y = y / value;
}

bool Vector2F::operator==(const float value) const
{
	return (x == value && y == value);
}

bool Vector2F::operator!=(const float value) const
{
	return (x != value || y != value);
}
