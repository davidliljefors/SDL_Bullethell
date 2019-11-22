#include "Projectile.h"

#include <Camera.h>
#include <Sprite.h>
#include <SDL_render.h>
#include <Circle.h>

#include <iostream>
#include "Player.h"
#include "Obstacle.h"
#include "Config.h"

Projectile::Projectile(FG::Sprite* sprite, float lifetime, bool playerFired, FG::Vector2D velocity, float accelerationSpeed,
	FG::Camera* camera) :
	lifetime(lifetime), playerFired(playerFired), velocity(velocity), accelerationSpeed(accelerationSpeed), camera(camera), maxBoundaries(Config::screenBoundaries)
{
	type = Regular;

	FG::Entity::sprite = sprite;
	AddCircleCollider(sprite->size.x / 2.f);

	if (playerFired)
		collisionLayer.set(1);
	else
	{
		collisionLayer.set(0);
		collisionLayer.set(4);
	}
	Reload();
}

Projectile::Projectile(FG::Sprite* sprite, float lifetime, bool playerFired, FG::Vector2D velocity, float accelerationSpeed,
	FG::Camera* camera, ProjectilePool* pool, Projectile* explosionProjectile, int projectileCount) :
	lifetime(lifetime), playerFired(playerFired), velocity(velocity), accelerationSpeed(accelerationSpeed), camera(camera), maxBoundaries(Config::screenBoundaries),
	pool(pool), explosionProjectile(explosionProjectile), projectileCount(projectileCount)
{
	type = Exploding;

	FG::Entity::sprite = sprite;
	AddCircleCollider(sprite->size.x / 2.f);

	Reload();
}

Projectile::Projectile(const Projectile& other)
{
	camera = other.camera;
	velocity = other.velocity;
	playerFired = other.playerFired;
	lifetime = other.lifetime;
	sprite = other.sprite;
	maxBoundaries = other.maxBoundaries;
	AddSprite(other.sprite);
	AddCircleCollider(sprite->size.x / 2.f);

	if (playerFired)
		collisionLayer.set(1);
	else
		collisionLayer.set(0);

	Reload();
}

Projectile::~Projectile()
{
}

void Projectile::Update(float deltaTime)
{
	isColliding = false;

	if (dead)
		return;

	Entity::Update(deltaTime);

	position += velocity * speedMult * deltaTime;

	speedMult += accelerationSpeed;

	if (position.x < -OFFSCREEN_LIMIT ||
		position.x > maxBoundaries.x + OFFSCREEN_LIMIT ||
		position.y < -OFFSCREEN_LIMIT ||
		position.y > maxBoundaries.y + OFFSCREEN_LIMIT)
		Reload();

	if (lifetime != -1)
	{
		elapsedTime += deltaTime;
		if (Expired())
			Reload();

		if (type == Exploding)
		{
			ExplodeProjectile();
		}
	}

}

void Projectile::Render(FG::Camera* const camera)
{
	if (dead)
		return;
	Entity::Render(camera);
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
	if (typeid(*other) == typeid(Obstacle)) {
		//Obstacle* ba = static_cast<Obstacle*>(other);
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

void Projectile::ExplodeProjectile()
{
	for (size_t i = 0; i < projectileCount; i++)
	{
		FG::Vector2D direction = FG::Vector2D::AngleToVector2D((360/projectileCount) * i);

		
	}
}

void Projectile::Fire(FG::Vector2D firePosition)
{
	position = firePosition;
	elapsedTime = 0.0f;
	dead = false;
}

void Projectile::SetValues(const Projectile& projectile)
{
	if (projectile.type == Regular)
	{
		sprite = projectile.sprite;
		lifetime = projectile.lifetime;
		velocity = projectile.velocity;
		accelerationSpeed = projectile.accelerationSpeed;
		camera = projectile.camera;
		maxBoundaries = projectile.maxBoundaries;
	}

	if (projectile.type == Exploding)
	{
		sprite = projectile.sprite;
		lifetime = projectile.lifetime;
		velocity = projectile.velocity;
		accelerationSpeed = projectile.accelerationSpeed;
		camera = projectile.camera;
		maxBoundaries = projectile.maxBoundaries;
		pool = projectile.pool;
		explosionProjectile = projectile.explosionProjectile;
		projectileCount = projectile.projectileCount;

	}
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
			(int)positions[i].x, (int)positions[i].y, (int)positions[i + 1].x, (int)positions[i + 1].y);
	}

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
#endif _DEBUG
}
