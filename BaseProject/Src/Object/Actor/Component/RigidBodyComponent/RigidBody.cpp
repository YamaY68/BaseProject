#include "RigidBody.h"

RigidBody::RigidBody()
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::SetBodyType(BodyType type)
{
	bodyType_ = type;
	// Ã“I„‘Ì‚Ìê‡A‹t¿—Ê‚ğ0‚Éİ’è
	if (type == BodyType::STATIC)
	{
		inverseMass_ = 0.0f;
	}
}

void RigidBody::SetMass(float mass)
{
	mass_ = mass;
	// ‹t¿—Ê‚ÌŒvZ
	if (mass_ != 0.0f)
	{
		inverseMass_ = 1.0f / mass_;
	}
	else
	{
		inverseMass_ = 0.0f; // ¿—Ê‚ª0‚Ìê‡A‹t¿—Ê‚à0‚Éİ’è
	}
}

void RigidBody::SetUseGravity(bool useGravity)
{
	useGravity_ = useGravity;
}

void RigidBody::SetVelocity(const VECTOR& velocity)
{
	velocity_ = velocity;
}

void RigidBody::SetGrounded(bool isGrounded)
{
	isGrounded_ = isGrounded;
}

void RigidBody::AddForce(const VECTOR& force)
{
	force_ = VAdd(force_, force);
}
