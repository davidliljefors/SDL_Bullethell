#include "Explosion.h"

Explosion::Explosion(FG::Sprite* anim)
{
	AddSprite(anim);
	
	layer = EntityLayer::Bullets;

	animation->Repeat(false);
	dead = true;
}

void Explosion::Update(float deltaTime)
{
	if (!animation->AnimationDone()) {
		animation->Update(deltaTime);
		if (animation->AnimationDone())
			dead = true;
	}
}

void Explosion::Render(FG::Camera* const camera)
{
	if (!dead && !animation->AnimationDone())
		Entity::Render(camera, scale);
}

void Explosion::Explode(FG::Vector2D pos, float s)
{
	this->position = pos;
	this->scale = s;
	animation->ReplayAnimation();
	dead = false;
}
