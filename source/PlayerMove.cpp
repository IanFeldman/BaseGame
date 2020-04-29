//
//  PlayerMove.cpp
//  Lab04
//
//  Created by Noah Feldman on 2/6/20.
//

#include "PlayerMove.h"
#include "SDL2/SDL.h"
#include <algorithm>
#include "Actor.h"
#include "Game.h"
#include "Block.h"
#include "Player.h"
#include "Fireball.h"

PlayerMove::PlayerMove(Actor* owner)
    :MoveComponent(owner)
    ,mYSpeed(0.0f)
    ,mSpacePressed(false)
    ,mInAir(true)
{
    mOwner = MoveComponent::mOwner;
    
    SetVelocity(Vector2::Zero);
    Vector2 acceleration(0.0f, 2000.0f);
    SetAcceleration(acceleration);
}

void PlayerMove::ProcessInput(const Uint8 *keyState)
{
    if ((keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_RIGHT]) || !(keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT])) {
        Vector2 velocity = GetVelocity();
        velocity.x = 0.0f;
        SetVelocity(velocity);
    }
    else if (keyState[SDL_SCANCODE_LEFT]) {
        Vector2 velocity = GetVelocity();
        velocity.x = -300.0f;
        SetVelocity(velocity);
    }
    else if (keyState[SDL_SCANCODE_RIGHT]) {
        Vector2 velocity = GetVelocity();
        velocity.x = 300.0f;
        SetVelocity(velocity);
    }
    if (keyState[SDL_SCANCODE_SPACE]) {
        if (!mSpacePressed && !mInAir) {
            Mix_PlayChannel(-1,mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
            Vector2 velocity = GetVelocity();
            velocity.y = -1000.0f;
            SetVelocity(velocity);
            mSpacePressed = true;
            mInAir = true;
        }
    }
    else {
        mSpacePressed = false;
    }
    if (keyState[SDL_SCANCODE_X]) {
        Fireball* fireball = new Fireball(mOwner->GetGame());
        fireball->SetPosition(mOwner->GetPosition());
        Vector2 velocity = GetVelocity();
        velocity.y = 0.0f;
        if (velocity.x >= 0)
            velocity.x += 500;
        else
            velocity.x -= 500;
        fireball->GetComponent<MoveComponent>()->SetVelocity(velocity);
        fireball->GetComponent<MoveComponent>()->SetAcceleration(Vector2::Zero);
        if (GetVelocity().x >= 0)
            fireball->GetComponent<AnimatedSprite>()->SetAnimation("RShoot");
        else 
            fireball->GetComponent<AnimatedSprite>()->SetAnimation("LShoot");
    }
}

void PlayerMove::Update(float deltaTime)
{
    SetVelocity(GetVelocity() + GetAcceleration()*deltaTime);
    Vector2 position(mOwner->GetPosition() + GetVelocity()*deltaTime);
    position.x = std::max(mOwner->GetGame()->GetCameraPos().x, position.x);
    mOwner->SetPosition(position);
    if (position.y > 448.0f) {
        //mOwner->GetGame()->GetPlayer()->SetDead(true);
    }
    else if (position.x > 6368.0f) {
        mOwner->SetState(ActorState::Paused);
        Mix_PlayChannel(-1,mOwner->GetGame()->GetSound("Assets/Sounds/StageClear.wav"), 0);
        Mix_HaltChannel(mOwner->GetGame()->GetPlayChannel());
    }
    else {
        CollisionComponent* thisCC = mOwner->GetComponent<CollisionComponent>();
        for (Block* block : mOwner->GetGame()->GetBlocks()) {
            CollisionComponent* otherCC = block->GetComponent<CollisionComponent>();
            if (thisCC->Intersect(otherCC)) {
                Vector2 offset;
                CollSide collside = thisCC->GetMinOverlap(otherCC, offset);
                if (collside == CollSide::Top && GetVelocity().y > 0) {
                    mInAir = false;
                    mYSpeed = 0.0f;
                    Vector2 velocity = GetVelocity();
                    velocity.y = 0.0f;
                    SetVelocity(velocity);
                    position += offset;
                    position.x = std::max(mOwner->GetGame()->GetCameraPos().x, position.x);
                    mOwner->SetPosition(position);
                } else if (collside != CollSide::None) {
                    position += offset;
                    position.x = std::max(mOwner->GetGame()->GetCameraPos().x, position.x);
                    mOwner->SetPosition(position);
                    if (collside == CollSide::Bottom) {
                        mYSpeed = 0.0f;
                        Vector2 velocity = GetVelocity();
                        velocity.y = 0.0f;
                        SetVelocity(velocity);
                        Mix_PlayChannel(-1,mOwner->GetGame()->GetSound("Assets/Sounds/Bump.wav"), 0);
                    }
                }
            }
        }
        
        for (Goomba* goomba : mOwner->GetGame()->GetEnemies()) {
            CollisionComponent* otherCC = goomba->GetComponent<CollisionComponent>();
            if (thisCC->Intersect(otherCC)) {
                Vector2 offset;
                CollSide collside = thisCC->GetMinOverlap(otherCC, offset);
                if (!goomba->IsStomped()) {
                    if (collside == CollSide::Top || ((collside == CollSide::Left || collside == CollSide::Right) && mInAir)) {
                        mInAir = true;
                        mYSpeed = -350.0f;
                        Vector2 velocity = GetVelocity();
                        velocity.y = -350.0f;
                        SetVelocity(velocity);
                        position += offset;
                        position.x = std::max(mOwner->GetGame()->GetCameraPos().x, position.x);
                        mOwner->SetPosition(position);
                        goomba->SetStomped(true);
                        Mix_PlayChannel(-1,mOwner->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
                    } else if (collside != CollSide::None) {
                        //mOwner->GetGame()->GetPlayer()->SetDead(true);
                        //break;
                    }
                }
            }
        }
    }
    
    AnimatedSprite* thisASprite = mOwner->GetComponent<AnimatedSprite>();
    if (mOwner->GetGame()->GetPlayer()->IsDead()) {
        thisASprite->SetAnimation("dead");
        mOwner->SetState(ActorState::Paused);
        Mix_PlayChannel(-1,mOwner->GetGame()->GetSound("Assets/Sounds/Dead.wav"), 0);
        Mix_HaltChannel(mOwner->GetGame()->GetPlayChannel());
    }
    else if (mInAir) {
        if (GetVelocity().x > 0) {
            thisASprite->SetAnimation("jumpRight");
        }
        else if (GetVelocity().x < 0) {
            thisASprite->SetAnimation("jumpLeft");
        }
        else {
            if (thisASprite->GetAnimName() == "runRight" || thisASprite->GetAnimName() == "jumpRight" || thisASprite->GetAnimName() == "idle") {
                thisASprite->SetAnimation("jumpRight");
            }
            else {
                thisASprite->SetAnimation("jumpLeft");
            }
        }
    }
    else {
        if (GetVelocity().x > 0) {
            thisASprite->SetAnimation("runRight");
        }
        else if (GetVelocity().x < 0) {
            thisASprite->SetAnimation("runLeft");
        }
        else {
            thisASprite->SetAnimation("idle");
        }
    }
    
    mYSpeed += 2000 * deltaTime;
    position.Set( mOwner->GetPosition().x - mOwner->GetGame()->GetWindowSize().x/2, mOwner->GetGame()->GetCameraPos().y);
    if (position.x > mOwner->GetGame()->GetCameraPos().x) {
        mOwner->GetGame()->SetCameraPos(position);
    }
}
