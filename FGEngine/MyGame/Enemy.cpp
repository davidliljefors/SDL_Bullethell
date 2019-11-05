#include "Enemy.h"
#include "Bullet.h"

#include <Camera.h>
#include <SDL_render.h>

Enemy::Enemy(FG::EntityManager& manager,FG::Camera* camera, FG::Vector2D pos) 
	: FG::Entity(manager), camera(camera)
{
	position = pos;
}

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

void Enemy::Shoot()
{
	Bullet* b = new Bullet(manager, camera, position, FG::Vector2D(0.f, -1.f));
	b->collider.SetSize(5);
	children.emplace_back(b);
	manager.AddEntity(b);
}

void Enemy::Update(float deltaTime)
{
	if (shotDelay < 0.0f)
	{
		Shoot();
		shotDelay = fireSpeed;
	}
	shotDelay -= deltaTime;
}