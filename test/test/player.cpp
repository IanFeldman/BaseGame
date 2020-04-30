#include "player.h"
#include "game.h"
#include "playermove.h"
#include "collisioncomponent.h"
#include "math.h"

Player::Player(Game* game)
	:Actor(game)
{
	SetScale(3.0f);
	mPlayerMove = new PlayerMove(this);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(32, 32);
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(mGame->GetTexture("Assets/lil-bricky.png"));
}

void Player::OnProcessInput(const Uint8* keyboardState)
{
}

void Player::OnUpdate(float deltaTime)
{
	Vector2 cameraPosition = mPosition;
	Vector2 offset;
	offset.Set(mGame->GetWindowSize().x / 2, mGame->GetWindowSize().y / 2);
	cameraPosition -= offset;
	mGame->SetCameraPosition(cameraPosition);
}