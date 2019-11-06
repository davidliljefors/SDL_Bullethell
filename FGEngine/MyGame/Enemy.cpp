#include "Enemy.h"

#include <Camera.h>
#include <SDL_render.h>

Enemy::Enemy(FG::Camera* camera, FG::Vector2D pos) : camera(camera), position(pos)
{}

void Enemy::Render(FG::Camera* const camera)
{
	SDL_Color oldDrawColor;
	SDL_Color color = SDL_Color{ 33, 33, 33, 255 };
	SDL_GetRenderDrawColor(camera->GetInternalRenderer(), &oldDrawColor.r, &oldDrawColor.g, &oldDrawColor.b, &oldDrawColor.a);
	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), color.r, color.g, color.b, color.a);

	FG::Vector2D finalPosition = position - camera->position;
	SDL_Rect finalRect = SDL_Rect{ (int)finalPosition.x, (int)finalPosition.y, 50, 50 };
	SDL_RenderFillRect(camera->GetInternalRenderer(), &finalRect);

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), oldDrawColor.r, oldDrawColor.g, oldDrawColor.b, oldDrawColor.a);
}
