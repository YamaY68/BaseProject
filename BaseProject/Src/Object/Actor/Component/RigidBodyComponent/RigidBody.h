#pragma once
#include<DxLib.h>
#include"../ComponentBase.h"
class RigidBody :public ComponentBase
{

public:

	RigidBody();
	~RigidBody();

	enum class BodyType
	{
		STATIC,		//静的
		DYNAMIC,	//動的
		KINEMATIC,	//キネマティック
	};

	void SetBodyType(BodyType type);	//剛体のタイプ設定
	void SetMass(float mass);				//質量設定
	void SetUseGravity(bool useGravity);			//重力を受けるかどうか設定
	void SetVelocity(const VECTOR& velocity);	//速度設定
	void SetGrounded(bool isGrounded);	//地面に接地しているかどうか設定
	void AddForce(const VECTOR& force);	//力を加える
	//移動速度設定
	void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
	void SetJumpPower(float power) { jumpPower_ = power; } //ジャンプ力設定

	//取得関数
	BodyType GetBodyType(void)const { return bodyType_; }	//剛体のタイプ
	float GetMass(void)const { return mass_; }				//質量
	float GetInverseMass(void)const { return inverseMass_; }//逆質量
	const VECTOR& GetVelocity(void)const { return velocity_; }//速度
	bool IsUseGravity(void)const { return useGravity_; }	//重力を受けるかどうか
	bool IsGrounded(void)const { return isGrounded_; }	//地面に接地しているかどうか
	VECTOR GetForce(void)const { return force_; }	//力

	//移動速度取得
	float GetMoveSpeed(void) const { return moveSpeed_; }
	float GetJumpPower(void) const { return jumpPower_; } //ジャンプ力取得


	void ClearForce(void) { force_ = VGet(0.0f, 0.0f, 0.0f); }

private:
	BodyType bodyType_;	//剛体のタイプ
	float mass_;	//質量
	float inverseMass_;	//逆質量
	VECTOR velocity_;	//速度
	VECTOR force_;	//力
	bool useGravity_=true;	//重力を受けるかどうか
	bool isGrounded_=false;	//地面に接地しているかどうか

	float moveSpeed_ = 5.0f; //移動速度
	float jumpPower_ = 10.0f; //ジャンプ力

	//float friction_;	//摩擦係数
	//float restitution_;	//反発係数


};

