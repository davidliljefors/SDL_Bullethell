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
	layer = EntityLayer::Character;
	
	// ADD NEW STUFF

	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 625.0f, camera),
			.02f, .5, .5, 1, 270, -22.5f, 22.5f, true) }),
			FG::Vector2D(pos.x, pos.y))
			);
	
	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 500.0f, camera),
			.075f, .75f, .5f, 3, 270, -22.5f, 22.5f, true ) }),
			FG::Vector2D(pos.x, pos.y), 16, false, .75f)
			);
	bossPhases[bossPhases.size() - 1]->AddPosition({ Config::SCREENWIDTH * .25, Config::SCREENHEIGHT * .25 });
	bossPhases[bossPhases.size() - 1]->AddPosition({ Config::SCREENWIDTH * .75, Config::SCREENHEIGHT * .25 });
	
	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, -250.0f, camera, -1, 12.5),
			.075f, 5, .5f, 5, 90, -30, 30, true)}),
			FG::Vector2D(pos.x, pos.y))
			);
	
	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 200, camera),
			.15f, 50, .5f, 20, 270, 0, 360, false) }),
		{ Config::SCREENWIDTH * .5, Config::SCREENHEIGHT * .5 }, 35, true, 2.5)
			);
	bossPhases[bossPhases.size() - 1]->AddPosition({ Config::SCREENWIDTH * .25, Config::SCREENHEIGHT * .5 });
	bossPhases[bossPhases.size() - 1]->AddPosition({ Config::SCREENWIDTH * .75, Config::SCREENHEIGHT * .5 });

	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 150, camera),
			.25f, 5, .5f, 30, 90, 0, 360, false, true, 10) }),
			FG::Vector2D(pos.x, pos.y), 70)
			);
	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 50.0f, camera, -1, 25),
			.075f, .75f, .5f, 5, 270, -80, 80, false, true, 5),
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 1000.0f, camera, -1, -5),
			.075f, .75f, .5f, 5, 90, -80, 80, false, true, 5) }),
			FG::Vector2D(pos.x, pos.y))
			);
	
	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 250, camera),
			.075f, 60, 0, 15, 90, 0, 360, false, true, 10) }),
			FG::Vector2D(pos.x, pos.y), 35)
			);
	
	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 500.0f, camera),
			.02f, 60, 0, 15, 90, -130, 130, false, false, 0),
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 500.0f, camera),
			.02f, 60, 0, 2, 270, -22.5, 22.5, false, false, 0),
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 750, camera),
			.02f, 1, .5, 1, 270, 0, 360, true, false, 0 ) }),
			FG::Vector2D( pos.x, pos.y ), 35)
			);
	
	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 250, camera),
			.001f, 60, 0, 5, 270, 0, 360, false, true, 1.5),
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 400.0f, camera),
			1.5f, 60, 0, 30, 270, 0, 360, false, false, 0) }),
		{ Config::SCREENWIDTH * .5, Config::SCREENHEIGHT * .5 }, 35)
			);

	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 1000.0f, camera),
			.02f, 60, 0, 105, 270, -90, 90, false, false, 5) }),
		{ Config::SCREENWIDTH * .5, Config::SCREENHEIGHT * .5 }, 105)
		);
	/*
	bossPhases.push_back(new BossPhase(
		std::vector<EmitterProperties*>({
			new EmitterProperties(*new Projectile(resourcecManager->GetResource<FG::Sprite>("BullethellBulletPurple.png"), false, FG::Vector2D::Up, 1000.0f, camera),
			.02f, 60, 0, 105, 90, -90, 90, false, true, 5) }),
		{ Config::SCREENWIDTH * .5, Config::SCREENHEIGHT * .5 },105)
		);
	bossPhases[bossPhases.size() - 1]->AddPosition({ Config::SCREENWIDTH * .25, Config::SCREENHEIGHT * .25 });
	bossPhases[bossPhases.size() - 1]->AddPosition({ Config::SCREENWIDTH * .75, Config::SCREENHEIGHT * .25 });
	bossPhases[bossPhases.size() - 1]->AddPosition({ Config::SCREENWIDTH * .25, Config::SCREENHEIGHT * .5 });
	bossPhases[bossPhases.size() - 1]->AddPosition({ Config::SCREENWIDTH * .75, Config::SCREENHEIGHT * .5 });

	*/
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
	if (health <= 0)
	{
		explosion->Explode(position, 4);
		EnterNextPhase();
		invincibleTime = 0;

		projectilePool->FadeOutAll();
		camera->Shake(1.5f);
	}
	isColliding = false;

	if (currentHitFlash > 0)
		currentHitFlash -= deltaTime;

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
			if ((position.y <= startPosition.y + 2.5 && position.y >= startPosition.y - 2.5) &&
				(position.x <= startPosition.x + 2.5 && position.x >= startPosition.x - 2.5)) {
				entersScreen = false;
			}
		}
	}
	else {


		if (position.y != destination.y || position.x != destination.x) {
			position = Lerp(position, destination, 5 * deltaTime);
			if ((position.y <= destination.y + 2.5 && position.y >= destination.y - 2.5) &&
				(position.x <= destination.x + 2.5 && position.x >= destination.x - 2.5)) {
				position = destination;
			}
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
		bool hasFired = false;
		if (bossPhases[currentBossPhase]->moveWhileFiring) {
			for (size_t i = 0; i < bossPhases[currentBossPhase]->emitters.size(); i++) {
				if (emitters[i]->Fire(deltaTime, player->position))
					hasFired = true;
			}
		}
		else if (position == destination) {
			for (size_t i = 0; i < bossPhases[currentBossPhase]->emitters.size(); i++) {
				if (emitters[i]->Fire(deltaTime, player->position))
					hasFired = true;
			}
		}
		if (hasFired)
			audioManager->PlaySFX("enemyFire.wav", 3);
		
		if (bossPhases[currentBossPhase]->positions.size() > 1) {
			if (position == destination)
				movePauseTime -= deltaTime;
			if (movePauseTime <= 0) {
				movePauseTime = bossPhases[currentBossPhase]->movePauseDuration;
				if (!bossPhases[currentBossPhase]->moveWhileFiring)
					ResetEmittersTime();
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
		movePauseTime = bossPhases[currentBossPhase]->movePauseDuration;
		destination = bossPhases[currentBossPhase]->positions[0];
		health = bossPhases[currentBossPhase]->health;

		for (size_t i = 0; i < bossPhases[currentBossPhase]->emitters.size(); i++)
			emitters[i]->SetEmitter(bossPhases[currentBossPhase]->emitters[i]);

		ResetEmittersTime();

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
		player->AwardReward();
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
	if(phase == Phase::dead)
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
	//barragePauseTime = 0;
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
