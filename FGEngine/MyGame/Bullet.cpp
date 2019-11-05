#include "Bullet.h"

#include <Camera.h>
#include <SDL_render.h>


Bullet::Bullet(FG::EntityManager& manager, FG::Camera* camera, FG::Vector2D pos, FG::Vector2D vel)
	: FG::Entity(manager), camera(camera), velocity(vel) 
{
	position = pos;
}

void Bullet::Render(FG::Camera* const camera)
{
	SDL_Color oldDrawColor;
	SDL_Color color = SDL_Color{ 255, 255, 0, 255 };
	SDL_GetRenderDrawColor(camera->GetInternalRenderer(), &oldDrawColor.r, &oldDrawColor.g, &oldDrawColor.b, &oldDrawColor.a);
	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), color.r, color.g, color.b, color.a);

	FG::Vector2D finalPosition = position - camera->position;
	SDL_Rect finalRect = SDL_Rect{ (int)finalPosition.x, (int)finalPosition.y, 10, 10 };
	SDL_RenderFillRect(camera->GetInternalRenderer(), &finalRect);

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), oldDrawColor.r, oldDrawColor.g, oldDrawColor.b, oldDrawColor.a);
}

void Bullet::Update(float deltaTime)
{
	position += velocity;
}
