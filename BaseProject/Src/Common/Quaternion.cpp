#include <math.h>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "Quaternion.h"

Quaternion::Quaternion(void) 
{
	//回転の量
	w = 1;
	//回転軸ベクトル
	x = y = z = 0;
}
Quaternion::Quaternion(const VECTOR& rad) 
{
	*this = Euler(rad);
}
Quaternion::Quaternion(double w, double x, double y, double z)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

Quaternion::~Quaternion(void) 
{
}
//オイラー角を元にクォータニオンを生成
Quaternion Quaternion::Euler(const VECTOR& rad)
{
	return Euler(rad.x, rad.y, rad.z);
}

/// <summary>
/// オイラー角をクォータニオンに変換
/// </summary>
/// <param name="radX">ロール</param>
/// <param name="radY">ピッチ</param>
/// <param name="radZ">ヨー</param>
/// <returns></returns>
Quaternion Quaternion::Euler(double radX, double radY, double radZ)
{
	Quaternion ret = Quaternion();
	//角度を0～2πに正規化
	radX = AsoUtility::RadIn2PI(radX);
	radY = AsoUtility::RadIn2PI(radY);
	radZ = AsoUtility::RadIn2PI(radZ);
	//各軸回転の準備
	double cosZ = cos(radZ / 2.0f);
	double sinZ = sin(radZ / 2.0f);
	double cosX = cos(radX / 2.0f);
	double sinX = sin(radX / 2.0f);
	double cosY = cos(radY / 2.0f);
	double sinY = sin(radY / 2.0f);

	//ret.w = cosZ * cosX * cosY + sinZ * sinX * sinY;
	//ret.x = sinZ * cosX * cosY - cosZ * sinX * sinY;
	//ret.y = cosZ * sinX * cosY + sinZ * cosX * sinY;
	//ret.z = cosZ * cosX * sinY - sinZ * sinX * cosY;

	//クォータニオンの生成
	ret.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
	ret.x = sinX * cosY * cosZ + cosX * sinY * sinZ;
	ret.y = cosX * sinY * cosZ - sinX * cosY * sinZ;
	ret.z = cosX * cosY * sinZ - sinX * sinY * cosZ;

	return ret;
}

/// <summary>
/// クウォータニオン同士の掛け算(ハルミトン算)
/// </summary>
/// <param name="q1"></param>
/// <param name="q2"></param>
/// <returns></returns>
Quaternion Quaternion::Mult(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion ret = Quaternion();
	double d1, d2, d3, d4;

	//w(回転量)の計算
	d1 = q1.w * q2.w;
	d2 = -q1.x * q2.x;
	d3 = -q1.y * q2.y;
	d4 = -q1.z * q2.z;
	ret.w = d1 + d2 + d3 + d4;

	//回転軸ベクトルの計算------------------------
	// xの計算 
	d1 = q1.w * q2.x;
	d2 = q2.w * q1.x;
	d3 = q1.y * q2.z;
	d4 = -q1.z * q2.y;
	ret.x = d1 + d2 + d3 + d4;

	// yの計算
	d1 = q1.w * q2.y;
	d2 = q2.w * q1.y;
	d3 = q1.z * q2.x;
	d4 = -q1.x * q2.z;
	ret.y = d1 + d2 + d3 + d4;

	// zの計算
	d1 = q1.w * q2.z;
	d2 = q2.w * q1.z;
	d3 = q1.x * q2.y;
	d4 = -q1.y * q2.x;
	ret.z = d1 + d2 + d3 + d4;
	//--------------------------------------------
	return ret;
}

/// <summary>
/// 2つのクォータニオンの掛け算
/// </summary>
/// <param name="q"></param>
/// <returns></returns>
Quaternion Quaternion::Mult(const Quaternion& q) const
{
	return Mult(Quaternion(w, x, y, z), q);
}

/// <summary>
/// 回転角と回転軸をもとにクォータニオンを生成
/// </summary>
/// <param name="rad回転角"></param>
/// <param name="axis">回転軸</param>
/// <returns></returns>
Quaternion Quaternion::AngleAxis(double rad, VECTOR axis)
{
	Quaternion ret = Quaternion();

	double norm;
	double c, s;

	// Unityに合わせる
	//ret.w = ret.x = ret.y = ret.z = 0.0;

	//初期化
	ret.w = 1.0;
	ret.x = ret.y = ret.z = 0.0;

	//回転軸の正規化
	norm = (double)axis.x * (double)axis.x + (double)axis.y * (double)axis.y + (double)axis.z * (double)axis.z;
	if (norm <= 0.0f)
	{
		//長さが０なら単位クォータニオンを返す
		return ret;
	}
	norm = 1.0 / sqrt(norm);
	axis.x = (float)(axis.x * norm);
	axis.y = (float)(axis.y * norm);
	axis.z = (float)(axis.z * norm);

	//回転角度を適用
	c = cos(0.5f * rad);
	s = sin(0.5f * rad);

	ret.w = c;
	ret.x = s * axis.x;
	ret.y = s * axis.y;
	ret.z = s * axis.z;

	return ret;
}

/// <summary>
/// 位置情報に回転情報を適応
/// </summary>
/// <param name="q"></param>
/// <param name="pos"></param>
/// <returns></returns>
VECTOR Quaternion::PosAxis(const Quaternion& q, VECTOR pos)
{
	//位置情報に回転情報を反映させる
	Quaternion tmp = Quaternion();
	tmp = tmp.Mult(q);
	//位置ベクトルをクォータニオンに変換
	tmp = tmp.Mult(Quaternion(0.0f,pos.x,pos.y,pos.z));
	//回転の適応と逆変換
	tmp = tmp.Mult(q.Inverse());
	return { (float)tmp.x, (float)tmp.y, (float)tmp.z };
}

/// <summary>
/// 新しい位置を計算する
/// </summary>
/// <param name="pos"></param>
/// <returns></returns>
VECTOR Quaternion::PosAxis(VECTOR pos) const
{
	return PosAxis(Quaternion(w, x, y, z), pos);
}

/// <summary>
/// クォータニオンをオイラー角に変換
/// </summary>
/// <param name="q"></param>
/// <returns></returns>
VECTOR Quaternion::ToEuler(const Quaternion& q)
{
	VECTOR ret;
	//回転行列の第１列と第３列の要素を計算
	double r11 = 2 * (q.x * q.z + q.w * q.y);
	//同じ列から回転行列の正規化した違いを摘出
	double r12 = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
	//回転行列のY軸の値
	double r21 = -2 * (q.y * q.z - q.w * q.x);
	//ｚ軸周りの回転要素を計算
	double r31 = 2 * (q.x * q.y + q.w * q.z);
	double r32 = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;
	//上下方向の回転角度を計算
	ret.x = static_cast<float>(asin(r21));
	//左右方向の回転角度を計算
	ret.y = static_cast<float>(atan2(r11, r12));
	//横方向の回転角度を計算
	ret.z = static_cast<float>(atan2(r31, r32));

	return ret;

}

//現在のクォータニオンをオイラー角に変換
VECTOR Quaternion::ToEuler(void) const
{
	return ToEuler(Quaternion(w, x, y, z));
}

/// <summary>
/// クォータニオンを回転行列に変換
/// </summary>
/// <param name="q"></param>
/// <returns></returns>
MATRIX Quaternion::ToMatrix(const Quaternion& q)
{

	MATRIX mat;

	FLOAT4 fq = { (float)q.x, (float)q.y, (float)q.z, (float)q.w };

	//クォータニオン成分の平方　回転行列のスケーリングに影響
	float sx = fq.x * fq.x * 2.0f;
	float sy = fq.y * fq.y * 2.0f;
	float sz = fq.z * fq.z * 2.0f;
	
	//軸の交差部分を計算し、回転方向を決定
	float cx = fq.y * fq.z * 2.0f;
	float cy = fq.x * fq.z * 2.0f;
	float cz = fq.x * fq.y * 2.0f;

	//成分を使った微調整
	float wx = fq.w * fq.x * 2.0f;
	float wy = fq.w * fq.y * 2.0f;
	float wz = fq.w * fq.z * 2.0f;

	mat.m[0][0] = 1.0f - (sy + sz);	mat.m[0][1] = cz + wz;			mat.m[0][2] = cy - wy;			mat.m[0][3] = 0.0f;
	mat.m[1][0] = cz - wz;			mat.m[1][1] = 1.0f - (sx + sz);	mat.m[1][2] = cx + wx;			mat.m[1][3] = 0.0f;
	mat.m[2][0] = cy + wy;			mat.m[2][1] = cx - wx;			mat.m[2][2] = 1.0f - (sx + sy);	mat.m[2][3] = 0.0f;
	mat.m[3][0] = 0.0f;				mat.m[3][1] = 0.0f;				mat.m[3][2] = 0.0f;				mat.m[3][3] = 1.0f;
	//mat.m[3][0] = trans.x;				mat.m[3][1] = trans.y;				mat.m[3][2] = trans.z;				mat.m[3][3] = 1.0f;

	return mat;

}

//現在のクォータニオンを回転行列に変換
MATRIX Quaternion::ToMatrix(void) const
{
	return ToMatrix(Quaternion(w, x, y, z));
}

Quaternion Quaternion::LookRotation(VECTOR dir)
{
	VECTOR up = { 0.0f, 1.0f, 0.0f };
	return LookRotation(dir, up);
}

/// <summary>
/// 方向ベクトルと上方向ベクトルを元にクォータニオンを生成
/// </summary>
/// <param name="dir"></param>
/// <param name="up"></param>
/// <returns></returns>
Quaternion Quaternion::LookRotation(VECTOR dir, VECTOR up)
{
	//正規化
	dir = AsoUtility::VNormalize(dir);
	//方向ベクトルと上方向ベクトルから左右ベクトルを計算
	VECTOR right = AsoUtility::VNormalize(VCross(up, dir));
	//新しい上方向ベクトルの再計算
	up = VCross(dir, right);
	//回転行列を構成する各要素を摘出
	auto m00 = right.x;
	auto m01 = right.y;
	auto m02 = right.z;
	auto m10 = up.x;
	auto m11 = up.y;
	auto m12 = up.z;
	auto m20 = dir.x;
	auto m21 = dir.y;
	auto m22 = dir.z;

	//トレース値を計算して分岐処理を最適化
	float num8 = (m00 + m11) + m22;
	auto quaternion = Quaternion();
	if (num8 > 0.0f)
	{
		double num = sqrt(num8 + 1.0);
		quaternion.w = num * 0.5;
		num = 0.5 / num;
		quaternion.x = ((double)m12 - m21) * num;
		quaternion.y = ((double)m20 - m02) * num;
		quaternion.z = ((double)m01 - m10) * num;
		return quaternion.Normalized();
	}
	if ((m00 >= m11) && (m00 >= m22))
	{
		// xとwが逆？
		//auto num7 = sqrt(((1.0f + m00) - m11) - m22);
		//auto num4 = 0.5f / num7;
		//quaternion.x = 0.5f * num7;
		//quaternion.y = (m01 + m10) * num4;
		//quaternion.z = (m02 + m20) * num4;
		//quaternion.w = (m12 - m21) * num4;
		//return quaternion.Normalized();
		auto num7 = sqrt(((1.0f + m00) - m11) - m22);
		auto num4 = 0.5f / num7;
		quaternion.x = ((double)m12 - m21) * num4;
		quaternion.y = ((double)m01 + m10) * num4;
		quaternion.z = ((double)m02 + m20) * num4;
		quaternion.w = 0.5 * num7;
		return quaternion.Normalized();
	}
	if (m11 > m22)
	{
		auto num6 = sqrt(((1.0f + m11) - m00) - m22);
		auto num3 = 0.5f / num6;
		quaternion.x = ((double)m10 + m01) * num3;
		quaternion.y = 0.5 * num6;
		quaternion.z = ((double)m21 + m12) * num3;
		quaternion.w = ((double)m20 - m02) * num3;
		return quaternion.Normalized();
	}
	auto num5 = sqrt(((1.0f + m22) - m00) - m11);
	auto num2 = 0.5f / num5;
	quaternion.x = ((double)m20 + m02) * num2;
	quaternion.y = ((double)m21 + m12) * num2;
	quaternion.z = 0.5 * num5;
	quaternion.w = ((double)m01 - m10) * num2;
	return quaternion.Normalized();

}

//回転行列をクォータニオンに変換
Quaternion Quaternion::GetRotation(MATRIX mat)
{
	Quaternion ret;

	float s;
	//行列トレースの計算
	float tr = mat.m[0][0] + mat.m[1][1] + mat.m[2][2] + 1.0f;
	//トレース値が１以上の場合
	if (tr >= 1.0f)
	{
		s = 0.5f / sqrtf(tr);
		//スカラーを基準にクォータニオンを算出
		ret.w = 0.25f / s;
		ret.x = (mat.m[1][2] - mat.m[2][1]) * s;
		ret.y = (mat.m[2][0] - mat.m[0][2]) * s;
		ret.z = (mat.m[0][1] - mat.m[1][0]) * s;
	}
	//トレース値が１未満の場合
	else
	{
		float max;
		//行列の要素の最大値に基づいて計算を分岐
		max = mat.m[1][1] > mat.m[2][2] ? mat.m[1][1] : mat.m[2][2];

		//ｘ成分を基準
		if (max < mat.m[0][0])
		{
			s = sqrtf(mat.m[0][0] - (mat.m[1][1] + mat.m[2][2]) + 1.0f);

			float x = s * 0.5f;
			s = 0.5f / s;
			ret.x = x;
			ret.y = (mat.m[0][1] + mat.m[1][0]) * s;
			ret.z = (mat.m[2][0] + mat.m[0][2]) * s;
			ret.w = (mat.m[1][2] - mat.m[2][1]) * s;
		}
		else
			//ｙ成分を基準
			if (max == mat.m[1][1])
			{
				s = sqrtf(mat.m[1][1] - (mat.m[2][2] + mat.m[0][0]) + 1.0f);

				float y = s * 0.5f;
				s = 0.5f / s;
				ret.x = (mat.m[0][1] + mat.m[1][0]) * s;
				ret.y = y;
				ret.z = (mat.m[1][2] + mat.m[2][1]) * s;
				ret.w = (mat.m[2][0] - mat.m[0][2]) * s;
			}
		//ｚ成分を基準
			else
			{
				s = sqrtf(mat.m[2][2] - (mat.m[0][0] + mat.m[1][1]) + 1.0f);

				float z = s * 0.5f;
				s = 0.5f / s;
				ret.x = (mat.m[2][0] + mat.m[0][2]) * s;
				ret.y = (mat.m[1][2] + mat.m[2][1]) * s;
				ret.z = z;
				ret.w = (mat.m[0][1] - mat.m[1][0]) * s;
			}
	}

	return ret;


	//float elem[4];
	//elem[0] = mat.m[0][0] - mat.m[1][1] - mat.m[2][2] + 1.0f;
	//elem[1] = -mat.m[0][0] + mat.m[1][1] - mat.m[2][2] + 1.0f;
	//elem[2] = -mat.m[0][0] - mat.m[1][1] + mat.m[2][2] + 1.0f;
	//elem[3] = mat.m[0][0] + mat.m[1][1] + mat.m[2][2] + 1.0f;

	//int biggestIdx = 0;
	//for (int i = 0; i < 4; i++)
	//{
	//    if (elem[i] > elem[biggestIdx])
	//    {
	//        biggestIdx = i;
	//    }
	//}

	//if (elem[biggestIdx] < 0)
	//{
	//    return Quaternion();
	//}

	//float q[4];
	//float v = sqrt(elem[biggestIdx]) * 0.5f;
	//q[biggestIdx] = v;
	//float mult = 0.25f / v;

	//switch (biggestIdx)
	//{
	//case 0:
	//    q[1] = (mat.m[1][0] + mat.m[0][1]) * mult;
	//    q[2] = (mat.m[0][2] + mat.m[2][0]) * mult;
	//    q[3] = (mat.m[2][1] - mat.m[1][2]) * mult;
	//    break;
	//case 1:
	//    q[0] = (mat.m[1][0] + mat.m[0][1]) * mult;
	//    q[2] = (mat.m[2][1] + mat.m[1][2]) * mult;
	//    q[3] = (mat.m[0][2] - mat.m[2][0]) * mult;
	//    break;
	//case 2:
	//    q[0] = (mat.m[0][2] + mat.m[2][0]) * mult;
	//    q[1] = (mat.m[2][1] + mat.m[1][2]) * mult;
	//    q[3] = (mat.m[1][0] - mat.m[0][1]) * mult;
	//    break;
	//case 3:
	//    q[0] = (mat.m[2][1] - mat.m[1][2]) * mult;
	//    q[1] = (mat.m[0][2] - mat.m[2][0]) * mult;
	//    q[2] = (mat.m[1][0] - mat.m[0][1]) * mult;
	//    break;
	//}

	//return Quaternion(q[3], q[0], q[1], q[2]);
}

VECTOR Quaternion::GetForward(void) const
{
	return GetDir(AsoUtility::DIR_F);
}

VECTOR Quaternion::GetBack(void) const
{
	return GetDir(AsoUtility::DIR_B);
}

VECTOR Quaternion::GetRight(void) const
{
	return GetDir(AsoUtility::DIR_R);
}

VECTOR Quaternion::GetLeft(void) const
{
	return GetDir(AsoUtility::DIR_L);
}

VECTOR Quaternion::GetUp(void) const
{
	return GetDir(AsoUtility::DIR_U);
}

VECTOR Quaternion::GetDown(void) const
{
	return GetDir(AsoUtility::DIR_D);
}

/// <summary>
/// 二つのクォータニオンの内積
/// </summary>
/// <param name="q1"></param>
/// <param name="q2"></param>
/// <returns></returns>
double Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
	return (q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z);
}

/// <summary>
/// 現在のクォータニオンと引数クォータニオンの内積
/// </summary>
/// <param name="q"></param>
/// <returns></returns>
double Quaternion::Dot(const Quaternion& q) const
{
	return (w * q.w + x * q.x + y * q.y + z * q.z);
}

/// <summary>
/// 指定したクォータニオンの正規化を返す
/// </summary>
/// <param name="q"></param>
/// <returns></returns>
Quaternion Quaternion::Normalize(const Quaternion& q)
{
	float scale = 1.0f / static_cast<float>(q.Length());
	VECTOR v = VScale(q.xyz(), scale);
	Quaternion ret = Quaternion(q.w * scale, v.x, v.y, v.z);
	return ret;
}

/// <summary>
/// 現在のクォータニオンの正規化を返す
/// </summary>
/// <param name=""></param>
/// <returns></returns>
Quaternion Quaternion::Normalized(void) const
{
	double mag = sqrt(w * w + x * x + y * y + z * z);
	return Quaternion(w / mag, x / mag, y / mag, z / mag);
}

/// <summary>
/// 現在のクォータニオンを正規化する
/// </summary>
/// <param name=""></param>
void Quaternion::Normalize(void)
{
	double mag = sqrt(w * w + x * x + y * y + z * z);

	w /= mag;
	x /= mag;
	y /= mag;
	z /= mag;

}

/// <summary>
/// クォータニオンの逆元を計算
/// </summary>
/// <param name=""></param>
/// <returns></returns>
Quaternion Quaternion::Inverse(void) const
{
	//ノルムを計算
	double n = 1.0f / (w * w + x * x + y * y + z * z);
	//共役クォータニオンの生成
	Quaternion tmp = Quaternion(w, -x, -y, -z);
	//逆元を返す
	return Quaternion(tmp.w * n, tmp.x * n, tmp.y * n, tmp.z * n);;
}

/// <summary>
/// 二つのクォータニオン間で球体に沿った補完
/// </summary>
/// <param name="from"></param>
/// <param name="to"></param>
/// <param name="t"></param>
/// <returns></returns>
Quaternion Quaternion::Slerp(Quaternion from, Quaternion to, double t)
{
	//補完係数の範囲チェックとクランプ
	if (t > 1) t = 1;
	if (t < 0) t = 0;
	return SlerpUnclamped(from, to, (float)t);

}

/// <summary>
/// 指定された２つのベクトル間のクォータニオンを返す
/// </summary>
/// <param name="fromDir"></param>
/// <param name="toDir"></param>
/// <returns></returns>
Quaternion Quaternion::FromToRotation(VECTOR fromDir, VECTOR toDir)
{
	//二つのベクトルの外積計算による回転軸を求める
	VECTOR axis = VCross(fromDir, toDir);
	//二つのベクトル間の角度を求める
	double angle = AsoUtility::AngleDeg(fromDir, toDir);
	//角度が180度以上の場合ベクトルが逆方向で回転軸が不安定になるため
	//回転軸を再計算
	if (angle >= 179.9196)
	{
		auto r = VCross(fromDir, AsoUtility::DIR_R);
		axis = VCross(r, fromDir);
		//if (axis.sqrMagnitude < 0.000001f)
		float len = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
		if (len < 0.000001f)
		{
			axis = AsoUtility::DIR_U;
		}
	}
	//回転軸の正規化(軸の長さを１に)
	axis = AsoUtility::VNormalize(axis);
	//クォータニオンの生成
	return Quaternion::AngleAxis(AsoUtility::Deg2RadD(angle), axis);

}

/// <summary>
/// 2つのクォータニオン間の回転を徐々に移動
/// </summary>
/// <param name="from"></param>
/// <param name="to"></param>
/// <param name="maxDegreesDelta">指定された最大角度</param>
/// <returns></returns>
Quaternion Quaternion::RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta)
{
	//二つのクォータニオンの間の角度を計算
	double num = Quaternion::Angle(from, to);
	//すでに到達しているためtoを返す
	if (num == 0.0)
	{
		return to;
	}
	float t = min(1.0f, maxDegreesDelta / (float)num);
	//指定された割合ｔに基づいて中間のクォータニオンを返す
	return Quaternion::SlerpUnclamped(from, to, t);
}

/// <summary>
/// 二つのクォータニオン間の角度を返す
/// </summary>
/// <param name="q1"></param>
/// <param name="q2"></param>
/// <returns></returns>
double Quaternion::Angle(const Quaternion& q1, const Quaternion& q2)
{
	//二つのクォータニオンの内積を計算
	double cos = Quaternion::Dot(q1, q2);
	//内積の値を逆余弦関数でクォータニオン間の角度をラジアンで取得
	double ac = acos(cos);
	//ラジアンデグリー変換
	return ac * (180.0 / DX_PI);
}

/// <summary>
/// 球面線形補完
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="t">補完パラメータ　0=a,1.0=b　</param>
/// <returns></returns>
Quaternion Quaternion::SlerpUnclamped(Quaternion a, Quaternion b, float t)
{

	//どちらかのクォータニオンが０の場合
	//０でないほうを返す
	if (a.LengthSquared() == 0.0f){
		//どちらも０の場合単位クォータニオンを返す
		if (b.LengthSquared() == 0.0f){
			return Identity();
		}
		return b;
	}
	else if (b.LengthSquared() == 0.0f){
		return a;
	}

	//クォータニオン間の内積計算
	float cosHalfAngle = (float)(a.w * b.w) + VDot(a.xyz(), b.xyz());

	if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f){
		// angle = 0.0f, so just return one input.
		return a;
	}
	//内積が負の時補完の方向を反転
	else if (cosHalfAngle < 0.0f){
		//b.xyz() = -b.xyz();
		b.x = b.x * -1.0f;
		b.y = b.y * -1.0f;
		b.z = b.z * -1.0f;
		b.w = -b.w;
		cosHalfAngle = -cosHalfAngle;
	}

	float blendA;
	float blendB;
	//角度が大きい場合球面線形補完
	if (cosHalfAngle < 0.99f){
		// do proper slerp for big angles
		float halfAngle = acosf(cosHalfAngle);
		float sinHalfAngle = sinf(halfAngle);
		float oneOverSinHalfAngle = 1.0f / sinHalfAngle;
		blendA = sinf(halfAngle * (1.0f - t)) * oneOverSinHalfAngle;
		blendB = sinf(halfAngle * t) * oneOverSinHalfAngle;
	}
	//角度が小さい場合線形補完
	else{
		blendA = 1.0f - t;
		blendB = t;
	}

	//各ベクトル成分に対してブレンド係数を適用して合成ベクトルを生成
	VECTOR v = VAdd(VScale(a.xyz(), blendA), VScale(b.xyz(), blendB));
	//Quaternion result = Quaternion(v.x, v.y, v.z, blendA * a.w + blendB * b.w);
	Quaternion result = Quaternion(blendA * a.w + blendB * b.w, v.x, v.y, v.z);
	//補完結果を正規化して有効なクォータニオンに変換
	if (result.LengthSquared() > 0.0f){
		return Normalize(result);
	}
	else{
		return Identity();
	}
}

/// <summary>
/// 単位クォータニオン
/// </summary>
/// <param name=""></param>
/// <returns></returns>
Quaternion Quaternion::Identity(void)
{
	return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

/// <summary>
/// クォータニオンのノルムを返す
/// </summary>
/// <param name=""></param>
/// <returns></returns>
double Quaternion::Length(void) const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

/// <summary>
/// クォータニオンの長さの２乗を返す
/// </summary>
/// <param name=""></param>
/// <returns></returns>
double Quaternion::LengthSquared(void) const
{
	return x * x + y * y + z * z + w * w;
}

/// <summary>
/// クォータニオンのベクトル部分を返す
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VECTOR Quaternion::xyz(void) const
{
	return { (float)x, (float)y, (float)z };
}

/// <summary>
/// クォータニオンを回転角度と回転軸に変換
/// </summary>
/// <param name="angle"></param>
/// <param name="axis"></param>
void Quaternion::ToAngleAxis(float* angle, VECTOR* axis)
{
	//クォータニオンのｗ成分が１を超えてる場合正規化
	if (abs(this->w) > 1.0f)
	{
		this->Normalize();
	}
	//ｗ成分から回転角度を求める
	*angle = 2.0f * acosf((float)this->w); // angle
	//クォータニオンが方向を持たない無回転の場合
	if (x == 0 && y == 0 && z == 0)
	{
		*angle = 0.0f;
	}
	//回転軸の計算
	float den = sqrtf(1.0f - (float)(this->w * this->w));
	if (den > 0.0001f)
	{
		//単位ベクトルを求める
		//axis = q->xyz / den;
		auto v = this->xyz();
		axis->x = v.x / den;
		axis->y = v.y / den;
		axis->z = v.z / den;
	}
	//denがほぼ０の場合回転角度が０に近い
	else
	{
		//任意の軸を指定
		// This occurs when the angle is zero. 
		// Not a problem: just set an arbitrary normalized axis.
		*axis = { 1.0f, 0.0f, 0.0f };
	}
}

VECTOR Quaternion::GetDir(VECTOR dir) const
{
	return PosAxis(dir);
}

Quaternion Quaternion::operator*(float& f) {
	return Quaternion(w * f, x * f, y * f, z * f);
}

const Quaternion Quaternion::operator*(const float& f) {
	return Quaternion(w * f, x * f, y * f, z * f);
}

Quaternion Quaternion::operator+(Quaternion& rhs) {
	return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
}

const Quaternion Quaternion::operator+(const Quaternion& rhs) {
	return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
}



