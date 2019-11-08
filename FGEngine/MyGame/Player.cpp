#include <iostream>

#include "Player.h"

#include <InputManager.h>
#include <Camera.h>
#include <SDL_render.h>
#include "Bullet.h"
#include <TextureManager.h>
#include <Sprite.h>

Player::Player(FG::EntityManager* manager, FG::InputManager* inputManager, FG::Camera* camera)
	: FG::Entity(manager), inputManager(inputManager), camera(camera)
{
	// todo set sprite
}

void Player::Update(float deltaTime)
{
	MovePlayer(deltaTime);
	MoveCamera(deltaTime);
	if (shotDelay < 0.0f && inputManager->IsKeyDown(SDL_SCANCODE_SPACE))
	{
		Shoot();
		shotDelay = fireSpeed;
	}
	shotDelay -= deltaTime;
}

void Player::Render(FG::Camera* const camera)
{
	Entity::Render(camera);

	/*SDL_Color oldDrawColor;
	SDL_Color color = SDL_Color{ 255, 0, 0, 255 };
	SDL_GetRenderDrawColor(camera->GetInternalRenderer(), &oldDrawColor.r, &oldDrawColor.g, &oldDrawColor.b, &oldDrawColor.a);
	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), color.r, color.g, color.b, color.a);

	FG::Vector2D finalPosition = position - camera->position;
	SDL_Rect finalRect = SDL_Rect{ (int)finalPosition.x, (int)finalPosition.y, 50, 50 };
	SDL_RenderFillRect(camera->GetInternalRenderer(), &finalRect);

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), oldDrawColor.r, oldDrawColor.g, oldDrawColor.b, oldDrawColor.a);*/
}

void Player::Shoot()
{
	Bullet* b = new Bullet(manager, camera, position, FG::Vector2D(0.f, -4.f));
	b->collider.groupID = this->collider.groupID;
	b->collider.SetSize(16);
	FG::Sprite* bulletS = new FG::Sprite(b, FG::TextureManager::GetTexture("assets/mild_panic.bmp"), 600, 600, 32, 32);
	b->SetSprite(bulletS);
	manager->AddEntity(b);

}

void Player::MovePlayer(float deltaTime)
{
	FG::Vector2D movement;
	if (inputManager->IsKeyDown(SDL_SCANCODE_A))
	{
		movement.x -= 1.0f;
	}
	if (inputManager->IsKeyDown(SDL_SCANCODE_D))
	{
		movement.x += 1.0f;
	}

	if (inputManager->IsKeyDown(SDL_SCANCODE_W))
	{
		movement.y -= 1.0f;
	}
	if (inputManager->IsKeyDown(SDL_SCANCODE_S))
	{
		movement.y += 1.0f;
	}

	position += movement * playerSpeed * deltaTime;
}

void Player::MoveCamera(float deltaTime)
{
	FG::Vector2D movement;
	if (inputManager->IsKeyDown(SDL_SCANCODE_LEFT))
	{
		movement.x -= 1.0f;
	}
	if (inputManager->IsKeyDown(SDL_SCANCODE_RIGHT))
	{
		movement.x += 1.0f;
	}

	if (inputManager->IsKeyDown(SDL_SCANCODE_UP))
	{
		movement.y -= 1.0f;
	}
	if (inputManager->IsKeyDown(SDL_SCANCODE_DOWN))
	{
		movement.y += 1.0f;
	}

	camera->position += movement * playerSpeed * deltaTime;
}
