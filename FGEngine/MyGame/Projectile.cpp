#include "Projectile.h"

#include <Camera.h>
#include <Sprite.h>
#include <SDL_render.h>
#include <Circle.h>

#include <iostream>
#include "Player.h"
#include "Obstacle.h"

Projectile::Projectile(FG::Sprite* sprite, float lifetime, bool playerFired, FG::Vector2D velocity, float accelerationSpeed,
	FG::Camera* camera, FG::Vector2D boundaries) :
	lifetime(lifetime), playerFired(playerFired), velocity(velocity), accelerationSpeed(accelerationSpeed), camera(camera), maxBoundaries(boundaries)
{
	type = Regular;

	FG::Entity::sprite = sprite;
	AddCircleCollider(sprite->size.x / 2.f);
	
	Reload();
}

Projectile::Projectile(FG::Sprite* sprite, float lifetime, bool playerFired, FG::Vector2D velocity, float accelerationSpeed, 
	FG::Camera* camera, FG::Vector2D boundaries, FG::EntityManager* entityManager, Projectile* explosionProjectile, int projectileCount) :
	lifetime(lifetime), playerFired(playerFired), velocity(velocity), accelerationSpeed(accelerationSpeed), camera(camera), maxBoundaries(boundaries),
	entityManager(entityManager), explosionProjectile(explosionProjectile), projectileCount(projectileCount)
{
	type = Exploding;

	FG::Entity::sprite = sprite;
	AddCircleCollider(sprite->size.x / 2.f);

	Reload();
}



Projectile::Projectile(const Projectile& other) :
	camera(other.camera), velocity(other.velocity), playerFired(other.playerFired), lifetime(other.lifetime), maxBoundaries(other.maxBoundaries)
{
	AddCircleCollider(sprite->size.x / 2.f);
	
	Reload();
}

void Projectile::Update(float deltaTime)
{
	//TODO Add acceleration
	//TODO Add Ro tat e.

	isColliding = false;

	if (dead)
		return;

	position += velocity * deltaTime;
	
	if (position.x < -OFFSCREEN_LIMIT ||
		position.x > maxBoundaries.x + OFFSCREEN_LIMIT ||
		position.y < -OFFSCREEN_LIMIT ||
		position.y > maxBoundaries.y + OFFSCREEN_LIMIT)
		Reload();

	if (lifetime != -1)
	{
		elapsedTime += deltaTime;

		if (Expired())
		{
			Reload();
			if (type == Exploding)
			{
				SpawnProjectileExplosion();
			}
		}
	}
}

void Projectile::Render(FG::Camera* const camera)
{
	if (dead)
		return;
	Entity::Render(camera);
	sprite->Render(camera, position);
	DrawColliderCircle();
}

SDL_Rect Projectile::GetColliderRect()
{
	FG::Vector2D finalPosition = position - camera->position;
	return { static_cast<int>(finalPosition.x), static_cast<int>(finalPosition.y),
	static_cast<int>(sprite->size.x), static_cast<int>(sprite->size.y) };
}

void Projectile::OnCollision(FG::Entity* other)
{
	/*if (typeid(other) == typeid(Obstacle)) {
		Obstacle* ba = static_cast<Obstacle*>(other);
	}*/

	if (typeid(*other) == typeid(Obstacle)) {
		Reload();
	}
}

bool Projectile::IgnoreCollision()
{
	return dead;
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

void Projectile::SpawnProjectileExplosion()
{
	//TODO EXPLODE LOL
	//Use Vector2D.AngleToVector2D() and make a circle based on how many projectileCount
	//MATH IS FUN
}

void Projectile::Move(float deltaTime)
{
	position += velocity * speedMult * deltaTime;
}

void Projectile::Fire(FG::Vector2D firePosition)
{
	position = firePosition;
	elapsedTime = 0.0f;
	dead = false;
}

void Projectile::Reload()
{
	dead = true;
}

void Projectile::DrawColliderCircle()
{
#ifdef _DEBUG
	const int samples = 10;
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
			positions[i].x, positions[i].y, positions[i + 1].x, positions[i + 1].y);
	}

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
#endif _DEBUG
}
