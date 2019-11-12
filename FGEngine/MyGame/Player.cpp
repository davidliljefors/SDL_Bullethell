#include "Player.h"

#include <InputManager.h>
#include <Camera.h>
#include <Sprite.h>
#include <SDL_render.h>
#include <Circle.h>

#include <cmath>

Player::Player(FG::InputManager* inputManager, FG::Camera* camera) :
	inputManager(inputManager), camera(camera)
{}

void Player::Update(float deltaTime)
{
	isColliding = false;
	MovePlayer(deltaTime);
	//MoveCamera(deltaTime);
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
	isColliding = true;
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

void Player::DrawBoundingBox()
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

void Player::DrawColliderCircle()
{
	SDL_Color color = notCollidingColor;
	if (isColliding)
	{
		color = CollidingColor;
	}
	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), color.r, color.g, color.b, color.a);
	FG::Vector2D positions[100];
	for (int i = 0; i < 100; i++)
	{
		positions[i].x = sin(i) * collider->GetRadius() + collider->GetPosition().x;
		positions[i].y = cos(i) * collider->GetRadius() + collider->GetPosition().y;
	}
	for (int i = 0; i < 99; i++)
	{
		SDL_RenderDrawLine(camera->GetInternalRenderer(),
			positions[i].x, positions[i].y, positions[i + 1].x, positions[i + 1].y);
	}

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
}
