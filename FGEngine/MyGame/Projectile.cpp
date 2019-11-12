#include "Projectile.h"

#include <Camera.h>
#include <Sprite.h>
#include <SDL_render.h>
#include <Circle.h>

Projectile::Projectile(float speed, float lifetime, bool playerFired, FG::Vector2D direction, FG::Camera* camera) :
	speed(speed), lifetime(lifetime), playerFired(playerFired), direction(direction), camera(camera) {}

void Projectile::Update(float deltaTime)
{
	isColliding = false;
	ProjectileUpdate();
}

void Projectile::Render(FG::Camera* const camera)
{
	Entity::Render(camera);
	sprite->Render(camera, position);
	DrawBoundingBox();
}

SDL_Rect Projectile::GetColliderRect()
{
	FG::Vector2D finalPosition = position - camera->position;
	return { static_cast<int>(finalPosition.x), static_cast<int>(finalPosition.y),
	static_cast<int>(sprite->size.x), static_cast<int>(sprite->size.y) };
}

void Projectile::OnCollision(FG::Entity* other)
{
	//TODO Destroy Projectile
}

void Projectile::DrawBoundingBox()
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

void Projectile::Move(float deltaTime)
{
	position += direction * speed * deltaTime;
}

void Projectile::ProjectileUpdate()
{
	//TODO if lifetime = -1 ignore it
	//TODO if count lifetime down with time and destroy projectile it once it hits 0
}

