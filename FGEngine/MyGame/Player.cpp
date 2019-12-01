
#include <cmath>

#include "Player.h"
#include <InputManager.h>
#include <EntityManager.h>
#include <ResourceManager.h>
#include <Camera.h>
#include <Sprite.h>
#include <SDL_render.h>
#include <Circle.h>
#include <SDL_events.h>

#include "GameState.h"
#include "Config.h"

Player::Player(FG::Vector2D pos, StateManager* stateManager, Projectile* projectile) :
	entityManager(stateManager->entityManager), inputManager(stateManager->inputManager), audioManager(stateManager->audioManager), camera(stateManager->camera), projectilePrefab(projectile),
	scoreController(stateManager->scoreController)
{
	minBoundaries = FG::Vector2D::Zero;
	maxBoundaries = Config::screenBoundaries;

	bomb = new Bomb(camera, position, stateManager->resourceManager->GetResource<FG::Sprite>("shockwave.png"));
	entityManager->AddEntity(bomb);

	SetUp();

	projectilePool = new ProjectilePool(MAX_BULLETS, projectilePrefab, entityManager);

	sensor = new Sensor(this, Sensor::graze, 25.f, stateManager->resourceManager->GetResource<FG::Sprite>("playercollider.png"));
	entityManager->AddEntity(sensor);

	explosion = new Explosion(stateManager->resourceManager->GetResource<FG::Sprite>("explo.png"));
	entityManager->AddEntity(explosion);

	collisionLayer.set(0);
	collisionLayer.set(5);
	audioManager->ChangeChannelVolume(.25f, 4);
	audioManager->ChangeChannelVolume(.5f, 5);
	audioManager->ChangeChannelVolume(.5f, 6);
	audioManager->ChangeChannelVolume(.125f, 7);

	StartPosition(pos);

	layer = EntityLayer::Character;
}

Player::~Player()
{
	projectilePool->Destroy();
}

void Player::Update(float deltaTime)
{
	explosion->Update(deltaTime);
	// Counter bomb mechanic
	if (hit)
	{
		hitFlashElapsedTime += deltaTime;
		if (hitFlashElapsedTime > hitFlashDuration)
		{
			hitFlashElapsedTime = 0;
			hitFlash = !hitFlash;
		}
		int slowMoEvent = SDL_RegisterEvents(1);
		if (slowMoEvent != ((Uint32)-1)) {
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event));
			event.type = slowMoEvent;
			event.user.code = 0xFFFF;
			SDL_PushEvent(&event);
		}
		timeSincelastHit += deltaTime;
		if (timeSincelastHit > counterbombTimeframe)
		{
			GetHit();
		}
	}
	if (lives < 0)
		return;

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
		if (bombs > 0 && inputManager->IsKeyPressed(SDL_SCANCODE_SPACE))
		{
			if (hit)
			{
				hit = false;
			}
			if (bomb->Activate(position, 0.5f))
			{
				invincibleTime -= .5f;
				if (bombs > 0) {
					bombs--;
					audioManager->PlaySFX("bomb.wav", 6);
					camera->Shake(2.5f);
				}
			}
		}
		if (fireTime <= 0 && inputManager->IsKeyDown(SDL_SCANCODE_Z))
		{
			fireTime = fireCooldown;
			Shoot();
		}
	}

	if (inputManager->IsKeyPressed(SDL_SCANCODE_Y))
	{
		godMode = !godMode;
	}
}

void Player::Render(FG::Camera* const camera)
{
	if (Respawning())
		return;
	assert(sprite);

	if (godMode)
	{
		sprite->Render(camera, position, SDL_Color({ 0,255,0 }), SDL_BLENDMODE_ADD);
		return;
	}

	if (hit)
	{

		if (hitFlash)
		{
			sprite->Render(camera, position, SDL_Color({ 255,75,75 }), SDL_BLENDMODE_BLEND);
		}
		else
		{
			sprite->Render(camera, position, SDL_Color({ 255,175,175 }), SDL_BLENDMODE_BLEND);
		}
	}
	else
	{
		sprite->Render(camera, position, (Invincible() ? (invincibleAlphaBlink ? 125 : 100) : 255));
	}



	assert(colliderSprite);
	colliderSprite->Render(camera, position);
#ifdef _DEBUG
	collider->Draw(camera, 0, 255, 0);
#endif _DEBUG
}

bool Player::AddSprite(FG::Sprite* sprite)
{
	assert(sprite);
	if (sprite)
	{
		this->sprite = sprite;
		straightSprite = sprite;
		return true;
	}

	return false;
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
	sprite = straightSprite;
	audioManager->PlaySFX("playerDestroyed.wav", 4);
	explosion->Explode(position, 2);
	PlaceOffscreenForEntrance();

	if (lives < 0) {

		camera->Shake(7.5f, 3.0f);
	}
	else {
		camera->Shake(5.0f, 2.0f);
		EnterScreen();
		Respawn();
	}
}

bool Player::OnGraze()
{
	if (IgnoreCollision())
		return false;
	scoreController->AddScore(25);

	audioManager->PlaySFX("graze.wav", 7);
	return true;
}

void Player::OnCollision(FG::Entity* other)
{
	if (!godMode)
	{
		isColliding = true;
		if (typeid(*other) == typeid(Sensor))
			return;

		if (bombs <= 0)
			GetHit();
		else if (!hit)
		{
			hit = true;
			timeSincelastHit = 0;
			audioManager->PlaySFX("hitAlarm.wav", 5);
		}
		hit = true;
		timeSincelastHit = 0;
		audioManager->PlaySFX("hitAlarm.wav", 5);
		rewardPlayer = false;
	}
}

bool Player::IgnoreCollision()
{
	return Invincible() || hit;
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
	if (lives >= 0)
		entersScreen = true;
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

void Player::AwardReward()
{
	if (rewardPlayer) {
		if (bombs >= maxBombs)
		{
			if (lives < maxLives)
				lives++;
		}
		else bombs++;
	}
	else rewardPlayer = true;
}

void Player::MovePlayer(float deltaTime)
{
	FG::Vector2D movement;
	float multiplier = 1;
	sprite = straightSprite;
	if (inputManager->IsKeyDown(SDL_SCANCODE_C))
	{
		multiplier *= focusMultiplier;
	}
	if (inputManager->IsKeyDown(SDL_SCANCODE_LEFT))
	{
		sprite = leftSprite;
		movement.x = -1.0f;
	}

	if (inputManager->IsKeyDown(SDL_SCANCODE_RIGHT))
	{
		sprite = rightSprite;
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
	sensor->position = position;
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
