#include <initializer_list>
#include <cmath>

#include "Player.h"
#include <InputManager.h>
#include <EntityManager.h>
#include <Camera.h>
#include <Sprite.h>
#include <SDL_render.h>
#include <Circle.h>

#include "GameState.h"
#include "Config.h"
#include <iostream>

Player::Player(FG::EntityManager* entityManager, FG::InputManager* inputManager, AudioManager* audioManager, FG::Camera* camera, Projectile* projectile) :
	entityManager(entityManager), inputManager(inputManager), audioManager(audioManager), camera(camera), projectilePrefab(projectile)
{
	minBoundaries = FG::Vector2D::Zero;
	maxBoundaries = Config::screenBoundaries;
	bomb = new Bomb(camera, position);
	entityManager->AddEntity(bomb);
	SetUp();

	projectilePool = new ProjectilePool(MAX_BULLETS, projectilePrefab, entityManager);

	collisionLayer.set(0);
	collisionLayer.set(5);

	audioManager->ChangeChannelVolume(.25f, 4);
	audioManager->ChangeChannelVolume(.5f, 5);
}

Player::~Player()
{
	projectilePool->Destroy();
}

void Player::Update(float deltaTime)
{
	// Counter bomb mechanic
	if (hit)
	{
		timeSincelastHit += deltaTime;
		if (timeSincelastHit > counterbombTimeframe)
		{
			GetHit();
		}
	}
	
	isColliding = false;
	if (State::state == GAME_STATES::start) {
		if (!firstBattle && lives >= 0) {
			float lerpSpeed = 2;
			if (position.y != startPosition.y || position.x != startPosition.x) {
				position = Lerp(position, startPosition, lerpSpeed * deltaTime);
			}
		}
		return;
	}

	if (fireTime > 0)
		fireTime -= deltaTime;

	if (Invincible()) {
		invincibleTime += deltaTime;

		invincibleAlphaBlinkTime -= deltaTime;
		if (invincibleAlphaBlinkTime <= 0) {
			invincibleAlphaBlinkTime = invincibleAlphaBlinkDuration - ((invincibleAlphaBlinkDuration / 1.25f) * (invincibleTime / invincibleDuration));
			invincibleAlphaBlink = !invincibleAlphaBlink;
		}
	}
	if (Respawning()) {
		respawnPauseTime -= deltaTime;
		return;
	}

	if (entersScreen) {
		if (position.y != startPosition.y || position.x != startPosition.x) {
			position = Lerp(position, startPosition, 10 * deltaTime);
			if ((position.y <= startPosition.y + 10 && position.y >= startPosition.y - 10) &&
				(position.x <= startPosition.x + 10 && position.x >= startPosition.x - 10)) {
				entersScreen = false;
			}
		}
	}
	else {

		MovePlayer(deltaTime);
		//MoveCamera(deltaTime);
		if (bombs > 0 && inputManager->IsKeyDown(SDL_SCANCODE_SPACE))
		{
			if (hit)
			{
				std::cout << "counter bomb\n";
				hit = false;
			}
			if (bomb->Activate(position, 0.5f))
			{
				if (bombs > 0)
					bombs--;
			}
		}
		if (fireTime <= 0 && inputManager->IsKeyDown(SDL_SCANCODE_Z))
		{
			fireTime = fireCooldown;
			Shoot();
		}
	}
}

void Player::Render(FG::Camera* const camera)
{
	if (Respawning())
		return;

	assert(sprite);
	sprite->Render(camera, position, (Invincible() ? (invincibleAlphaBlink ? 125 : 100) : 255));

	assert(colliderSprite);
	colliderSprite->Render(camera, position);
	DrawColliderCircle();
}

SDL_Rect Player::GetColliderRect()
{
	FG::Vector2D finalPosition = position - camera->position;
	return { static_cast<int>(finalPosition.x), static_cast<int>(finalPosition.y),
	static_cast<int>(sprite->size.x), static_cast<int>(sprite->size.y) };
}

//Used by counter bomb mechanic
void Player::GetHit() 
{
	hit = false;
	lives--;
	
	audioManager->PlaySFX("playerDestroyed.wav", 4);
	PlaceOffscreenForEntrance();
	if (lives < 0) {
		State::state = start;
	}
	else {
		EnterScreen();
		Respawn();
	}
}

void Player::OnCollision(FG::Entity* other)
{
	isColliding = true;

	if (!hit)
	{
		hit = true;
		timeSincelastHit = 0;
		audioManager->PlaySFX("hitAlarm.wav", 5);
	}

	//lives--;

	//PlaceOffscreenForEntrance();
	//if (lives < 0) {
	//	State::state = start;
	//}
	//else {
	//	EnterScreen();
	//	Respawn();
	//}
}

bool Player::IgnoreCollision()
{
	return Invincible();
}

void Player::StartPosition(FG::Vector2D pos)
{
	position = startPosition = pos;
	PlaceOffscreenForEntrance();
}

void Player::EnterScreen()
{
	entersScreen = true;
}

void Player::PlaceOffscreenForEntrance()
{
	position = startPosition - FG::Vector2D::Down * 250;
}

void Player::OnVictory()
{
	entersScreen = true;
	//SetUp();
}

void Player::OnStartBattle()
{
	SetUp();
	EnterScreen();
	firstBattle = false;
}

int Player::CurrentLives()
{
	return lives;
}

int Player::CurrentBombs()
{
	return bombs;
}

void Player::MovePlayer(float deltaTime)
{
	FG::Vector2D movement;
	float multiplier = 1;

	if (inputManager->IsKeyDown(SDL_SCANCODE_C))
	{
		multiplier *= focusMultiplier;
	}
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

	position += movement * multiplier * speed * deltaTime;

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

void Player::SetUp()
{
	lives = maxLives;
	bombs = maxBombs;

	fireTime = 0;
	invincibleTime = invincibleDuration;
}

void Player::Respawn()
{
	respawnPauseTime = respawnPauseDuration;
	invincibleTime = 0 - respawnPauseDuration;

	invincibleAlphaBlinkTime = invincibleAlphaBlinkDuration;
	invincibleAlphaBlink = false;
}

void Player::Shoot()
{
	Projectile* proj = projectilePool->GetProjectile();
	if (proj) {
		proj->Fire(position + FG::Vector2D(0, -15));
		audioManager->PlaySFX("fire.wav");
	}
	/*for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (projectiles[i]->Dead())
		{
			projectiles[i]->Fire(position + FG::Vector2D(0,-15));
			break;
		}
	}*/
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
			(int)positions[i].x, (int)positions[i].y, (int)positions[i + 1].x, (int)positions[i + 1].y);
	}

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
#endif _DEBUG
}
