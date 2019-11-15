#include "Entity.h"
#include "Circle.h"

void FG::Entity::Render(Camera* const camera)
{
	assert(sprite);
	sprite->Render(camera, position);
}

bool FG::Entity::IgnoreCollision()
{
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
