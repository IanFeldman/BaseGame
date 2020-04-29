//
//  Player.h
//  Lab04
//
//  Created by Noah Feldman on 2/6/20.
//

#pragma once
#include "Actor.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"

class Player : public Actor
{
public:
    Player(class Game* game);
    void SetAnim(std::string animName);
    
    void SetDead(bool dead);
    bool IsDead() { return mDead; }
    
private:
    AnimatedSprite* mASprite;
    CollisionComponent* mCollisionComponent;
    PlayerMove* mPlayerMove;
    bool mDead;
    
};
