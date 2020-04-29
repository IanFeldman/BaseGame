//
//  Player.cpp
//  Lab04
//
//  Created by Noah Feldman on 2/6/20.
//

#include "Player.h"
#include "Game.h"
#include "PlayerMove.h"

Player::Player(Game* game)
    :Actor(game)
    ,mDead(false)
{
    mASprite = new AnimatedSprite(this, 200);
    std::vector<SDL_Texture*> idleAnim{
    GetGame()->GetTexture("Assets/SlugBoy/WalkR0.png")
    };
    std::vector<SDL_Texture*> deadAnim{
    GetGame()->GetTexture("Assets/SlugBoy/WalkR0.png")
    };
    std::vector<SDL_Texture*> jumpLeftAnim{
    GetGame()->GetTexture("Assets/SlugBoy/WalkR0.png")
    };
    std::vector<SDL_Texture*> jumpRightAnim{
    GetGame()->GetTexture("Assets/SlugBoy/WalkR0.png")
    };
    std::vector<SDL_Texture*> runLeftAnim{
    GetGame()->GetTexture("Assets/SlugBoy/WalkL0.png"),
    GetGame()->GetTexture("Assets/SlugBoy/WalkL1.png")
    };
    std::vector<SDL_Texture*> runRightAnim{
    GetGame()->GetTexture("Assets/SlugBoy/WalkR0.png"),
    GetGame()->GetTexture("Assets/SlugBoy/WalkR1.png")
    };
    
    mASprite->AddAnimation("idle", idleAnim);
    mASprite->AddAnimation("dead", deadAnim);
    mASprite->AddAnimation("jumpLeft", jumpLeftAnim);
    mASprite->AddAnimation("jumpRight", jumpRightAnim);
    mASprite->AddAnimation("runLeft", runLeftAnim);
    mASprite->AddAnimation("runRight", runRightAnim);
    mASprite->SetAnimation("idle");
    
    mCollisionComponent = new CollisionComponent(this);
    mCollisionComponent->SetSize(32.0f, 32.0f);
    
    mPlayerMove = new PlayerMove(this);
}

void Player::SetAnim(std::string animName)
{
    mASprite->SetAnimation(animName);
}

void Player::SetDead(bool dead)
{
    mDead = dead;
}
