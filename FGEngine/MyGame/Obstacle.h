#pragma once
#include "EntityManager.h"
#include "Vector2D.h"
#include <SDL_pixels.h>
#include "Projectile.h"
#include "ProjectilePool.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include <vector>

static constexpr int s_HealthValues[4] = {30,50,75,9999};

namespace FG
{
	class Sprite;
	class Camera;
}

enum class Phase
{
	first,
	second,
	third,
	dead
};

class Obstacle : public FG::Entity
{
public:
	void Initialize();
	void Update(float deltaTime) override;
	Obstacle(FG::Vector2D pos, FG::EntityManager* eManager, AudioManager* aManager, FG::ResourceManager* rManager, FG::Camera* camera);
	void Render(FG::Camera* const camera) override;
	void EnterNextPhase();

	SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;
	bool IgnoreCollision() override;
	bool AddSprite(FG::Sprite* spr) override;

	void EnterScreen();
	void PlaceOffscreenForEntrance();
	Phase CurrentPhase();
	void SetUp();
	int GetHealth() { return health; }
	int GetMaxHealth() { return s_HealthValues[static_cast<int>(phase)]; }
	bool Invincible() { return invincibleTime < invincibleDuration; }
private:

	int health = s_HealthValues[0];
	int currentMaxHealth = s_HealthValues[0];
	Phase phase = Phase::first;

	static constexpr float firePauseDuration = .2f;
	static constexpr float barrageDuration = 1.0f;
	static constexpr float barragePauseDuration = 2.0f;

	static constexpr float invincibleDuration = 1.0f;
	static constexpr float invincibleAlphaBlinkDuration = .1f;
	
	float firePauseTime;
	float barrageTime;
	float barragePauseTime;

	float invincibleTime;
	float invincibleAlphaBlinkTime;
	bool invincibleAlphaBlink;
	static constexpr float hitFlashDuration = 0.07f;
	float currentHitFlash = 0;

	std::vector<std::vector<FG::Vector2D>> bossPositions;


	bool isColliding = false;
	static constexpr SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	static constexpr SDL_Color CollidingColor = { 255, 0, 0, 255 };
	
	FG::EntityManager* entityManager;
	FG::ResourceManager* resourcecManager;
	AudioManager* audioManager;

	FG::Camera* camera = nullptr;
	std::vector<FG::Sprite*> sprites;

	bool entersScreen;
	bool firstBattle = true;

	FG::Vector2D startPosition;
	FG::Vector2D destination;

	ProjectilePool* projectilePool = nullptr;

	void StartPosition(FG::Vector2D pos);
	void OnDeath();
	void Fire();
	void MoveToAnotherPosition();
};


