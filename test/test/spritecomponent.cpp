#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	Vector2 ownerPos = mOwner->GetPosition();
	Vector2 camPos = mOwner->GetGame()->GetCameraPosition();
	SDL_Point windowSize = mOwner->GetGame()->GetWindowSize();

	// Dont draw what is outside of the camera
	if ((ownerPos.x < camPos.x && ownerPos.y < camPos.y) || (ownerPos.x > camPos.x + windowSize.x && ownerPos.y > camPos.y + windowSize.y))
		return;

	if (mTexture)
	{
		SDL_Rect r;
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2 - mOwner->GetGame()->GetCameraPosition().x);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2 - mOwner->GetGame()->GetCameraPosition().y);

		// Draw (have to convert angle from radians to degrees, and clockwise to counter)
		SDL_RenderCopyEx(renderer,
			mTexture,
			nullptr,
			&r,
			-Math::ToDegrees(mOwner->GetRotation()),
			nullptr,
			SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	// Set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}
