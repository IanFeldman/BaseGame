#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner)
    :Component(owner, 50)
    ,mAngularSpeed(0.0f)
    ,mVelocity(Vector2::Zero)
    ,mAcceleration(Vector2::Zero)
{
    mOwner = Component::mOwner;
}

void MoveComponent::Update(float deltaTime)
{
    mVelocity += mAcceleration * deltaTime;

	mOwner->SetRotation((deltaTime * mAngularSpeed) + mOwner->GetRotation());
    mOwner->SetPosition((deltaTime * mVelocity) + mOwner->GetPosition());
}
