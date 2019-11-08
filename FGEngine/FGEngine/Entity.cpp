#include "Entity.h"
#include "Camera.h"

void FG::Entity::Render(Camera* const camera)
{
	if (sprite != nullptr)
	{
		camera->DrawSprite(sprite);
	}
}
