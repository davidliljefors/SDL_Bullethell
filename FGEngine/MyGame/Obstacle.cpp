#include "Obstacle.h"
#include <Sprite.h>
#include <Camera.h>
#include <SDL_render.h>

void Obstacle::Update(float deltaTime)
{
	isColliding = false;
}

void Obstacle::Render(FG::Camera* const camera)
{
	Entity::Render(camera);
	DrawBoundingBox();
}

SDL_Rect Obstacle::GetColliderRect()
{
	FG::Vector2D finalPosition = position - camera->position;
	return { static_cast<int>(finalPosition.x), static_cast<int>(finalPosition.y),
	static_cast<int>(sprite->size.x), static_cast<int>(sprite->size.y) };
}

void Obstacle::OnCollision(FG::Entity* other)
{
	isColliding = true;
}

void Obstacle::DrawBoundingBox()
{
	SDL_Color color = notCollidingColor;
	if (isColliding)
	{
		color = CollidingColor;
	}

	SDL_Rect finalRect = GetColliderRect();
	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), color.r, color.g, color.b, color.a);

	SDL_RenderDrawRect(camera->GetInternalRenderer(), &finalRect);
	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
}
