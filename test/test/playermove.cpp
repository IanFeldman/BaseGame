#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "player.h"
#include "collisioncomponent.h"
#include <Windows.h>
#include <iostream>

PlayerMove::PlayerMove(Actor* owner)
    :MoveComponent(owner)
{

}

void PlayerMove::Update(float deltaTime)
{
    mOwner->SetRotation((deltaTime * GetAngularSpeed()) + mOwner->GetRotation());
    SetVelocity(deltaTime * GetAccel() + GetVelocity());
    mOwner->SetPosition((deltaTime * GetVelocity()) + mOwner->GetPosition());
}

void PlayerMove::ProcessInput(const Uint8* keyboardState)
{
    Vector2 direction = Vector2::Zero;

    bool left = keyboardState[SDL_SCANCODE_A];
    bool right = keyboardState[SDL_SCANCODE_D];
    bool up = keyboardState[SDL_SCANCODE_W];
    bool down = keyboardState[SDL_SCANCODE_S];
    bool leftright = !(left && right) && !(!left && !right);
    bool updown = !(up && down) && !(!up && !down);
    if (leftright || updown) {
        if (left) {
            direction.x--;
        }
        if (right) {
            direction.x++;
        }
        if (up) {
            direction.y--;
        }
        if (down) {
            direction.y++;
        }

        SetVelocity(Vector2::Normalize(direction) * mSpeed);
    }
    else {
        SetVelocity(Vector2::Zero);
    }
}