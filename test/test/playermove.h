#pragma once
#include "MoveComponent.h"
#include <Windows.h>
#include "SDL2/SDL_stdinc.h"

class PlayerMove : public MoveComponent
{
public:
    PlayerMove(class Actor* owner);
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyboardState) override;

private:
    float mSpeed = 350.0f;
};