#include <typeinfo>
#include <Sprite.h>
#include <Camera.h>
#include <SDL_render.h>
#include <cassert>
#include <Timer.h>

#include "GameState.h"
#include "Obstacle.h"
#include "Player.h"
#include "Config.h"
#include "EntityManager.h"

#include <iostream>

Obstacle::Obstacle(FG::Vector2D pos, StateManager* stateManager) : entityManager(stateManager->entityManager), audioManager(stateManager->audioManager), resourcecManager(stateManager->resourceManager),
camera(stateManager->camera), entersScreen(false), scoreController(stateManager->scoreController), player(stateManager->player)
{
	collisionLayer.set(1);
	collisionLayer.set(0);

	explosion = new Explosion(resourcecManager->GetResource<FG::Sprite>("explo.png"));
	entityManager->AddEntity(explosion);

	projectilePool = new ProjectilePool(1000, new Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 1000.f, camera), entityManager);
	audioManager->ChangeChannelVolume(.25f, 3);
	audioManager->ChangeChannelVolume(.5f, 2);

	invincibleAlphaBlinkTime = invincibleAlphaBlinkDuration;
	invincibleAlphaBlink = false;
	invincibleTime = invincibleDuration * .5f;

	StartPosition(pos);

	for (size_t i = 0; i < static_cast<int>(Phase::dead); i++)
	{
		bossPositions.push_back({ {pos.x, pos.y} });
	}
	bossPositions[0].push_back({ Config::SCREENWIDTH * .25, Config::SCREENHEIGHT * .25 });
	bossPositions[0].push_back({ Config::SCREENWIDTH * .75, Config::SCREENHEIGHT * .25 });

	bossPositions[1].push_back({ Config::SCREENWIDTH * .25, Config::SCREENHEIGHT * .25 });
	bossPositions[1].push_back({ Config::SCREENWIDTH * .75, Config::SCREENHEIGHT * .25 });

	bossPositions[2].push_back({ Config::SCREENWIDTH * .25, Config::SCREENHEIGHT * .25 });
	bossPositions[2].push_back({ Config::SCREENWIDTH * .75, Config::SCREENHEIGHT * .25 });
	destination = bossPositions[0][0];
	
	layer = EntityLayer::Character;

	emitter = new Emitter(position, stateManager);
}

void Obstacle::Initialize()
{

	Entity::AddSprite(sprites[static_cast<int>(phase)]);
}

void Obstacle::Update(float deltaTime)
{
	explosion->Update(deltaTime);
	if (health <= 0)
	{
		EnterNextPhase();
		invincibleTime = 0;

		barrageTime = barrageDuration;
		barragePauseTime = barragePauseDuration;
		firePauseTime = firePauseDuration;
		projectilePool->ReloadAll();

		explosion->Explode(position);
	}
	isColliding = false;

	if (currentHitFlash > 0)
	{
		currentHitFlash -= deltaTime;
	}

	if (State::state == GAME_STATES::start) {
		if (!firstBattle && (phase != Phase::dead)) {
			float lerpSpeed = 2;
			if (position.y != startPosition.y || position.x != startPosition.x) {
				position = Lerp(position, startPosition, lerpSpeed * deltaTime);
			}
		}
		return;
	}

	if (entersScreen) {
		if (position.y != startPosition.y || position.x != startPosition.x) {
			position = Lerp(position, startPosition, 5 * deltaTime);
			if ((position.y <= startPosition.y + 5 && position.y >= startPosition.y - 5) &&
				(position.x <= startPosition.x + 5 && position.x >= startPosition.x - 5)) {
				entersScreen = false;
			}
		}
	}
	else {


		if (position.y != destination.y || position.x != destination.x) {
			position = Lerp(position, destination, 1.5f * deltaTime);
			/*if ((position.y <= destination.y + 5 && position.y >= destination.y - 5) &&
				(position.x <= destination.x + 5 && position.x >= destination.x - 5)) {
				position
			}*/
		}

		if (Invincible()) {
			invincibleTime += deltaTime;

			invincibleAlphaBlinkTime -= deltaTime;
			if (invincibleAlphaBlinkTime <= 0) {
				invincibleAlphaBlinkTime = invincibleAlphaBlinkDuration - ((invincibleAlphaBlinkDuration / 1.25f) * (invincibleTime / invincibleDuration));
				invincibleAlphaBlink = !invincibleAlphaBlink;
			}
			return;
		}

		if (barragePauseTime > 0) {
			barragePauseTime -= deltaTime;
			if (barragePauseTime <= 0)
				barrageTime = barrageDuration;
		}
		else {
			if (firePauseTime > 0) {
				/*firePauseTime -= deltaTime;
				if (firePauseTime <= 0) {
					Fire();
				}*/
			}
			else
				firePauseTime = firePauseDuration;

			barrageTime -= deltaTime;
			if (barrageTime <= 0) {
				barragePauseTime = barragePauseDuration;
				if (bossPositions[static_cast<int>(phase)].size() > 1)
					MoveToAnotherPosition();
			}
		}
	}
	emitter->Move(position);
	Projectile* newBullet = new Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 500, camera);
;
	if (emitter->Fire(*newBullet, deltaTime, 15, -90, 90, 2, .2f, FireModes::Linear))
		;
	delete newBullet;

}


void Obstacle::Fire()
{
	Vector2D directionToPlayer = player->position;
	Projectile* newBullet = new Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 1000.f, camera, projectilePool,
		new Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Down, 500, camera, 5, 2), 6, .5f);
	
	Projectile* proj = projectilePool->GetProjectile();
	*proj = *newBullet;
	delete newBullet;
	
	if (proj) {
		proj->Fire(position + FG::Vector2D(0, -15));
		audioManager->PlaySFX("enemyFire.wav", 3);
	}
}

void Obstacle::MoveToAnotherPosition()
{
	FG::Vector2D newDestination;
	do
		newDestination = bossPositions[static_cast<int>(phase)][rand() % bossPositions.size()];
	while (newDestination == destination);
	destination = newDestination;
}


void Obstacle::Render(FG::Camera* const camera)
{
	if (currentHitFlash > 0)
	{
		sprite->Render(camera, position, { 255, 255, 255 });
	}
	else
	{
		sprite->Render(camera, position, (Invincible() ? (invincibleAlphaBlink ? 125 : 100) : 255));
	}

#ifdef _DEBUG
	collider->Draw(camera, 255, 0, 0);
#endif _DEBUG
}

void Obstacle::EnterNextPhase()
{
	assert(phase != Phase::dead);
	phase = static_cast<Phase>(static_cast<int>(phase) + 1);
	assert(sprites.size() > static_cast<int>(Phase::dead)); // Make sure we have enough sprites for all phasess
	Entity::AddSprite(sprites[static_cast<int>(phase)]);
	switch (phase)
	{
	case Phase::first:
		//should never happen
		break;
	case Phase::second:
		health = currentMaxHealth = s_HealthValues[1];
		std::cout << "Enter second phase" << std::endl;
		break;
	case Phase::third:
		health = currentMaxHealth = s_HealthValues[2];
		std::cout << "Enter Third Phase" << std::endl;
		break;
	case Phase::dead:
		// die
		health = currentMaxHealth = s_HealthValues[3];
		OnDeath();
		std::cout << "boss died" << std::endl;
		break;
	default:
		break;
	}
	if (phase != Phase::dead)
		destination = bossPositions[static_cast<int>(phase)][0];
}

SDL_Rect Obstacle::GetColliderRect()
{
	FG::Vector2D finalPosition = position - camera->position;
	return { static_cast<int>(finalPosition.x - (sprite->size.x / 2)), static_cast<int>(finalPosition.y - (sprite->size.y / 2)),
		static_cast<int>(sprite->size.x), static_cast<int>(sprite->size.y) };
}

void Obstacle::OnCollision(FG::Entity* other)
{
	if (typeid(*other) == typeid(Projectile)) {

		currentHitFlash = hitFlashDuration;

		Projectile* p = static_cast<Projectile*>(other);
		health -= 1;

		if (health <= 0)
		{
			audioManager->PlaySFX("enemyHurt.wav", 2);

			scoreController->AddScore(1000);
		}
		else
		{
			if (health <= (currentMaxHealth / 6))
			{
				audioManager->PlaySFX("hit1.wav", 1);
			}
			else
				audioManager->PlaySFX("hit.wav", 1);
			scoreController->AddScore(500);
		}
	}
	isColliding = true;
}

bool Obstacle::IgnoreCollision()
{
	return Invincible();
}

void Obstacle::OnDeath()
{
	health = currentMaxHealth = 999;
	entersScreen = false;
	PlaceOffscreenForEntrance();
}

void Obstacle::StartPosition(FG::Vector2D pos)
{
	position = startPosition = pos;
	PlaceOffscreenForEntrance();
}
void Obstacle::EnterScreen()
{
	entersScreen = true;
	SetUp();
	firstBattle = false;
}

void Obstacle::PlaceOffscreenForEntrance()
{
	position = startPosition - FG::Vector2D::Up * 250;
}

Phase Obstacle::CurrentPhase()
{
	return phase;
}

void Obstacle::SetUp()
{
	phase = Phase::first;
	health = currentMaxHealth = s_HealthValues[0];
	Entity::AddSprite(sprites[static_cast<int>(phase)]);
	barrageTime = barrageDuration;
	barragePauseTime = 0;
	firePauseTime = firePauseDuration;
}

bool Obstacle::AddSprite(FG::Sprite* spr)
{
	assert(spr);
	sprites.push_back(spr);
	return true;
}
