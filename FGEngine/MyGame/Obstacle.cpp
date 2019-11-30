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

	projectilePool = new ProjectilePool(10000, new Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 1000.f, camera), entityManager);
	audioManager->ChangeChannelVolume(.25f, 3);
	audioManager->ChangeChannelVolume(.5f, 2);

	invincibleAlphaBlinkTime = invincibleAlphaBlinkDuration;
	invincibleAlphaBlink = false;
	invincibleTime = invincibleDuration * .5f;

	StartPosition(pos);
	/*
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
	*/
	layer = EntityLayer::Character;

	emitter = new Emitter(position, projectilePool, stateManager);
	//emitter->SetEmitter(*new Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 1000.0f, camera), 1);
	
	// ADD NEW STUFF

	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 1000.0f, camera),
			.1f, 10, 2, 10, 270, 0, 360, true, 3) } ),
			FG::Vector2D( pos.x, pos.y ))
	);
	bossPhases[bossPhases.size() - 1]->AddPosition({ Config::SCREENWIDTH * .25, Config::SCREENHEIGHT * .25 });
	bossPhases[bossPhases.size() - 1]->AddPosition({ Config::SCREENWIDTH * .75, Config::SCREENHEIGHT * .25 });
	
	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 1000.0f, camera),
			.2f, 10, 2, 10,270, 0, 360, false, 0, false),
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 1000.0f, camera),
			.02f, 1, 2, 1, 270, 0, 360, false, 0, true) }),
			FG::Vector2D( pos.x, pos.y ),35, 0)
			);
	
	// CALCULATING STUFF

	int totalEmitters = 1;
	for (auto& bp : bossPhases)	{
		if (bp->emitters.size() > totalEmitters)
			totalEmitters = (int)bp->emitters.size();
	}
	for (size_t i = 0; i < totalEmitters; i++)
	{
		emitters.push_back(new Emitter(position, projectilePool, stateManager));
	}

	destination = bossPhases[0]->positions[0];

	health = bossPhases[currentBossPhase]->health;

	for (size_t i = 0; i < bossPhases[currentBossPhase]->emitters.size(); i++)
		emitters[i]->SetEmitter(bossPhases[currentBossPhase]->emitters[i]);
}

void Obstacle::Initialize()
{
	Entity::AddSprite(sprites[static_cast<int>(phase)]);
}

void Obstacle::Update(float deltaTime)
{
	explosion->Update(deltaTime);
	//emitter->Move(position);
	if (health <= 0)
	{
		EnterNextPhase();
		invincibleTime = 0;

		projectilePool->ReloadAll();

		camera->Shake(1.5f);
		explosion->Explode(position, 4);
	}
	isColliding = false;


	if (currentHitFlash > 0)
	{
		currentHitFlash -= deltaTime;
	}

	if (phase == Phase::dead)
		return;

	for (size_t i = 0; i < bossPhases[currentBossPhase]->emitters.size(); i++)
		emitters[i]->Move(position);

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
				;// barrageTime = barrageDuration;
		}
		else {
			/*
			if (firePauseTime > 0) {
				firePauseTime -= deltaTime;
				if (firePauseTime <= 0) {
					Fire(Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 1000.0f, camera),
						360 * barrageTime/barrageDuration, false, 6, 0, 180);

					//Fire(Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 1000.0f, camera),
						//360, true, 7, -45, 45);
					//Fire(Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 100.0f, camera),
						//0, true, 60, 0, 360);
				}
			}
			else
				firePauseTime = firePauseDuration;
			*/

			bool emittersDone = true;
			for (auto& e : emitters) {

				e->Fire(deltaTime, player->position);
				if (!e->FinishedBarrage())
					emittersDone = false;
			}

			if (emittersDone)
			{
				barragePauseTime = bossPhases[currentBossPhase]->barragePauseDuration;
				ResetEmittersTime();
				//if (bossPositions[static_cast<int>(phase)].size() > 1)
				if (bossPhases[currentBossPhase]->positions.size() > 1)
					MoveToAnotherPosition();
			}
		}
	}

}

void Obstacle::ResetEmittersTime()
{
	for (size_t i = 0; i < bossPhases[currentBossPhase]->emitters.size(); i++)
		emitters[i]->ResetTime();
}

void Obstacle::Fire(Projectile projectile, float angle, bool targetPlayer, int bullets, float minAngle, float maxAngle)
{
	if (targetPlayer)
	{
		angle = (player->position - position).GetAngle();
	}

	emitter->SetAngle(angle);
	//emitter->Fire(bullets, minAngle, maxAngle);

	audioManager->PlaySFX("enemyFire.wav", 3);
}

void Obstacle::MoveToAnotherPosition()
{
	FG::Vector2D newDestination;
	do
		newDestination = bossPhases[currentBossPhase]->positions[rand() % bossPhases[currentBossPhase]->positions.size()];
	while (newDestination == destination);
	destination = newDestination;
}


void Obstacle::Render(FG::Camera* const camera)
{
	if (State::state == GAME_STATES::game)
		bottomIndicator->Render(camera, { position.x, indicatorYOffset },(Invincible()? 128: 255));
	
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
	//assert(phase != Phase::dead);
	//phase = static_cast<Phase>(static_cast<int>(phase) + 1);
	//assert(sprites.size() > static_cast<int>(Phase::dead)); // Make sure we have enough sprites for all phasess

	currentBossPhase++;
	if (currentBossPhase < bossPhases.size() ) {
		destination = bossPhases[currentBossPhase]->positions[0];
		health = bossPhases[currentBossPhase]->health;
		barragePauseTime = bossPhases[currentBossPhase]->barragePauseDuration;

		for (size_t i = 0; i < bossPhases[currentBossPhase]->emitters.size(); i++)
			emitters[i]->SetEmitter(bossPhases[currentBossPhase]->emitters[i]);

		ResetEmittersTime();

		//Entity::AddSprite(sprites[static_cast<int>(phase)]);
		float progress = ((float)currentBossPhase / bossPhases.size());
		float spriteStage = (float)1 / sprites.size();
		for (size_t i = 0; i < sprites.size(); i++)
		{
			float currentStage = (i+1) * spriteStage;
			if (progress <= currentStage) {
				Entity::AddSprite(sprites[i]);
				break;
			}
		}

		//std::cout << ((float)currentBossPhase / bossPhases.size()) << std::endl;
		//std::cout << ((float)1 / sprites.size()) << std::endl;
		//std::cout << progress << " " << (currentBossPhase)* spriteStage << std::endl;
		std::cout << currentBossPhase << std::endl;

	}
	else {
		OnDeath();
	}
}
#include <iostream>


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
	phase = Phase::dead;
	currentBossPhase = 0;
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

int Obstacle::GetCurrentPhase()
{
	return currentBossPhase;
}

int Obstacle::GetPhaseCount()
{
	return static_cast<int>(bossPhases.size());
}

void Obstacle::SetUp()
{
	phase = Phase::first;
	health = currentMaxHealth = bossPhases[0]->health;
	Entity::AddSprite(sprites[static_cast<int>(phase)]);
	barragePauseTime = 0;
	ResetEmittersTime();
	currentBossPhase = -1;
	EnterNextPhase();
}

bool Obstacle::AddSprite(FG::Sprite* spr)
{
	assert(spr);
	sprites.push_back(spr);
	return true;
}
