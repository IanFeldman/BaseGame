#pragma once
#include "actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const Uint8* keyboardState) override;

private:
	class PlayerMove* mPlayerMove;
	class SpriteComponent* mSpriteComponent;
	class CollisionComponent* mCollisionComponent;
	std::vector<class Actor*> mPlayerKeys;
};