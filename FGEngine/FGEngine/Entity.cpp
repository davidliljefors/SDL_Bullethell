#include "Entity.h"

void FG::Entity::Render(Camera* const camera)
{
	assert(sprite);
	sprite->Render(camera, position);
}
