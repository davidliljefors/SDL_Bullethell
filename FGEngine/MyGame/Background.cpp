#include "Background.h"
void Background::Update(float deltaTime)
{

	if (position.y >= 1.5f * sprite->size.y)
	{
		position.y -= (sprite->size.y * 2);
	}
	position.y += speed * deltaTime;
}
