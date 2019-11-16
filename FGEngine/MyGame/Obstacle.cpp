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
	DrawColliderCircle();
	//DrawBoundingBox();
}

SDL_Rect Obstacle::GetColliderRect()
{
	FG::Vector2D finalPosition = position - camera->position;
	return { static_cast<int>(finalPosition.x - (sprite->size.x / 2)), static_cast<int>(finalPosition.y - (sprite->size.y / 2)),
		static_cast<int>(sprite->size.x), static_cast<int>(sprite->size.y) };
}

void Obstacle::OnCollision(FG::Entity* other)
{
	isColliding = true;
}

void Obstacle::DrawBoundingBox()
{
#ifdef _DEBUG
	SDL_Color color = notCollidingColor;
	if (isColliding)
	{
		color = CollidingColor;
	}

	SDL_Rect finalRect = GetColliderRect();
	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), color.r, color.g, color.b, color.a);

	SDL_RenderDrawRect(camera->GetInternalRenderer(), &finalRect);
	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
#endif _DEBUG
}

void Obstacle::DrawColliderCircle()
{
#ifdef _DEBUG
	assert(collider);
	const int samples = 100;
	SDL_Color color = notCollidingColor;
	if (isColliding)
	{
		color = CollidingColor;
	}
	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), color.r, color.g, color.b, color.a);
	FG::Vector2D positions[samples + 1];
	for (int i = 0; i < samples + 1; i++)
	{
		positions[i].x = sin(360.f / samples * i * 3.14159f / 180.f) * collider->GetRadius() + position.x;
		positions[i].y = cos(360.f / samples * i * 3.14159f / 180.f) * collider->GetRadius() + position.y;
	}
	for (int i = 0; i < samples; i++)
	{
		SDL_RenderDrawLine(camera->GetInternalRenderer(),
			(int)positions[i].x, (int)positions[i].y, (int)positions[i + 1].x, (int)positions[i + 1].y);
	}

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
#endif _DEBUG
}