//
//  PlayerMove.h
//  Lab04
//
//  Created by Noah Feldman on 2/6/20.
//

#pragma once

#include "MoveComponent.h"
#include "Math.h"

class PlayerMove : public MoveComponent
{
public:
    PlayerMove(class Actor* owner);
    virtual void ProcessInput(const Uint8 *keyState);
    virtual void Update(float deltaTime);
    
protected:
    class Actor* mOwner;
    
private:
    float mYSpeed;
    bool mSpacePressed, mInAir;
    
};
