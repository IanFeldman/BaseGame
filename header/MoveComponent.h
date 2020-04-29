#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner);

	// Update the move component
	void Update(float deltaTime) override;
	
	// Getters/setters
	float GetAngularSpeed() const { return mAngularSpeed; }
	Vector2 GetVelocity() const { return mVelocity; }
	Vector2 GetVelocityNorm() const { return Vector2::Normalize(mVelocity); }
	Vector2 GetAcceleration() const { return mAcceleration; }
	Vector2 GetAccelerationNorm() const { return Vector2::Normalize(mVelocity); }

	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
    void SetVelocity(Vector2 velocity) { mVelocity = velocity; }
	void SetAcceleration(Vector2 acceleration) { mAcceleration = acceleration;  }
    
private:
	// Angular speed (in radians/second)
	float mAngularSpeed;
    // Direction of movement
	Vector2 mVelocity;
	Vector2 mAcceleration;
};
