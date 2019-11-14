#include <initializer_list>
#include <cmath>

#include "Player.h"
#include <InputManager.h>
#include <EntityManager.h>
#include <Camera.h>
#include <Sprite.h>
#include <SDL_render.h>
#include <Circle.h>


Player::Player(FG::EntityManager* entityManager, FG::InputManager* inputManager, FG::Camera* camera, FG::Vector2D boundaries, Projectile* projectile) :
	entityManager(entityManager), inputManager(inputManager), camera(camera), projectilePrefab(projectile)
{
	minBoundaries = FG::Vector2D::Zero;
	maxBoundaries = boundaries;

	lives = maxLives;

	fireTime = 0;

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		projectiles[i] = new Projectile(*projectilePrefab);
		entityManager->AddEntity(projectiles[i]);
	}

	//entityManager->AddEntities(projectiles, MAX_BULLETS);
}

Player::~Player()
{
	for (int i = 0; i < MAX_BULLETS; i++)
		projectiles[i] = NULL;
}

void Player::Update(float deltaTime)
{
	isColliding = false;
	MovePlayer(deltaTime);
	//MoveCamera(deltaTime);

	if (fireTime > 0)
		fireTime -= deltaTime;
	else {
		if (inputManager->IsKeyDown(SDL_SCANCODE_SPACE))
		{
			fireTime = fireCooldown;
			Shoot();
		}
	}

}

void Player::Render(FG::Camera* const camera)
{
	Entity::Render(camera);
	sprite->Render(camera, position);
	DrawColliderCircle();
}

SDL_Rect Player::GetColliderRect()
{
	FG::Vector2D finalPosition = position - camera->position;
	return { static_cast<int>(finalPosition.x), static_cast<int>(finalPosition.y),
	static_cast<int>(sprite->size.x), static_cast<int>(sprite->size.y) };
}

void Player::OnCollision(FG::Entity* other)
{
	if (other)
		isColliding = true;
}

bool Player::IgnoreCollision()
{
	return false;
}

void Player::MovePlayer(float deltaTime)
{
	FG::Vector2D movement;
	if (inputManager->IsKeyDown(SDL_SCANCODE_LEFT))
	{
		movement.x = -1.0f;
	}

	if (inputManager->IsKeyDown(SDL_SCANCODE_RIGHT))
	{
		movement.x = 1.0f;
	}

	if (inputManager->IsKeyDown(SDL_SCANCODE_UP))
	{
		movement.y = -1.0f;
	}

	if (inputManager->IsKeyDown(SDL_SCANCODE_DOWN))
	{
		movement.y = 1.0f;
	}

	position += movement * speed * deltaTime;

	if (position.x < minBoundaries.x + sprite->size.x / 2)
		position.x = minBoundaries.x + sprite->size.x / 2;
	else if (position.x > maxBoundaries.x - sprite->size.x / 2)
		position.x = maxBoundaries.x - sprite->size.x / 2;
	if (position.y < minBoundaries.y + sprite->size.y / 2)
		position.y = minBoundaries.y + sprite->size.y / 2;
	else if (position.y > maxBoundaries.y - sprite->size.y / 2)
		position.y = maxBoundaries.y - sprite->size.y / 2;
}

void Player::MoveCamera(float deltaTime)
{
	FG::Vector2D movement;
	if (inputManager->IsKeyDown(SDL_SCANCODE_LEFT))
	{
		movement.x = -1.0f;
	}

	if (inputManager->IsKeyDown(SDL_SCANCODE_RIGHT))
	{
		movement.x = 1.0f;
	}

	if (inputManager->IsKeyDown(SDL_SCANCODE_UP))
	{
		movement.y = -1.0f;
	}

	if (inputManager->IsKeyDown(SDL_SCANCODE_DOWN))
	{
		movement.y = 1.0f;
	}

	camera->position += movement * speed * deltaTime;
}

void Player::Shoot()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (projectiles[i]->Dead())
		{
			projectiles[i]->Fire(position);
			break;
		}
	}
	/*Projectile* proj = new Projectile(*projectilePrefab);
	proj->position = position;
	entityManager->AddEntity(proj);*/
}

void Player::DrawBoundingBox()
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

void Player::DrawColliderCircle()
{
#ifdef _DEBUG
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
			positions[i].x, positions[i].y, positions[i + 1].x, positions[i + 1].y);
	}

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
#endif _DEBUG
}
