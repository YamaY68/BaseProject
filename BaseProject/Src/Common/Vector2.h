#pragma once

class Vector2
{
public:

	int x;
	int y;

	// コンストラクタ
	Vector2(void);

	// コンストラクタ
	Vector2(int vX, int vY);

	// デストラクタ
	~Vector2(void);

	//演算

	// 代入---------------------------------------
	void operator=(const int value);
	//--------------------------------------------

	// 加算---------------------------------------
	Vector2 operator+(const Vector2 value)const;
	void operator+=(const Vector2 value);

	Vector2 operator+(const int value)const;
	void operator+=(const int value);
	//--------------------------------------------

	// 減算---------------------------------------
	Vector2 operator-(const Vector2 value)const;
	void operator-=(const Vector2 value);

	Vector2 operator-(const int value)const;
	void operator-=(const int value);
	//--------------------------------------------

	// 乗算---------------------------------------
	Vector2 operator*(const Vector2 value)const;
	void operator*=(const Vector2 value);

	Vector2 operator*(const int value)const;
	void operator*=(const int value);
	//--------------------------------------------

	// 除算---------------------------------------
	Vector2 operator/(const Vector2 value)const;
	void operator/=(const Vector2 value);

	Vector2 operator/(const int value)const;
	void operator/=(const int value);
	//--------------------------------------------
};

class Vector2F
{
public:

	float x;
	float y;

	//コンストラクタ
	Vector2F(void);

	//コンストラクタ
	Vector2F(float vX, float vY);

	//デストラクタ
	~Vector2F(void);

	//演算

	Vector2F operator+(const Vector2F value)const;
	void operator+=(const Vector2F value);
	Vector2F operator-(const Vector2F value)const;
	void operator-=(const Vector2F value);
	Vector2F operator*(const float value)const;
	void operator*=(const float value);
	Vector2F operator/(const float value)const;
	void operator/=(const float value);

	bool operator==(const float value)const;
	bool operator!=(const float value)const;
};