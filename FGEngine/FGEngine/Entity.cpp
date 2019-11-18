#include "Entity.h"
#include "Circle.h"

void FG::Entity::Update(float deltaTime)
{
	if (animation)
	{
		animation->Update(deltaTime);
	}
}

void FG::Entity::Render(Camera* const camera)
{
	assert(sprite);
	sprite->Render(camera, position, animation->GetFrame());
}

bool FG::Entity::IgnoreCollision()
{
	return false;
}

bool FG::Entity::AddSprite(Sprite* sprite)
{
	if (sprite)
	{
		if (sprite->GetFrameCount() > 1)
		{
			this->animation = new Animation(sprite);
		}
		this->sprite = sprite;
		return true;
	}
	return false;
}


void FG::Entity::AddCircleCollider(float radius)
{
	collider = new FG::Circle(&position, radius);
}

void FG::Entity::AddCircleCollider(float offsetX, float offsetY, float radius)
{
	collider = new FG::Circle(&position, offsetX, offsetY, radius);
}


FG::Vector2D FG::Entity::Lerp(FG::Vector2D& start, FG::Vector2D& end, float time) {
	if (time <= 0.0f)
		return start;

	if (time >= 1.0f)
		return end;

	FG::Vector2D dir = (end - start).Normalized();
	float mag = (end - start).Magnitude();

	return start + dir * mag * time;
}