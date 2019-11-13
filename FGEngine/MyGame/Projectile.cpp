#include "Projectile.h"

#include <Camera.h>
#include <Sprite.h>
#include <SDL_render.h>
#include <Circle.h>

#include <iostream>

Projectile::Projectile(FG::Sprite* sprite, float lifetime, bool playerFired, FG::Vector2D velocity, FG::Camera* camera) :
	lifetime(lifetime), playerFired(playerFired), velocity(velocity), camera(camera)
{
	FG::Entity::sprite = sprite;
	AddCircleCollider(sprite->size.x / 2.f);
}

Projectile::Projectile(const Projectile& other)
{
	camera = other.camera;
	velocity = other.velocity;
	playerFired = other.playerFired;
	lifetime = other.lifetime;
	sprite = other.sprite;
	AddCircleCollider(sprite->size.x / 2.f);
}

void Projectile::Update(float deltaTime)
{
	elapsedTime += deltaTime;
	isColliding = false;
	//ProjectileUpdate();
	position += velocity * deltaTime;
}

void Projectile::Render(FG::Camera* const camera)
{
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
	position += velocity * deltaTime;
}

void Projectile::ProjectileUpdate()
{
	//TODO if lifetime = -1 ignore it
	//TODO if count lifetime down with time and destroy projectile it once it hits 0
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
