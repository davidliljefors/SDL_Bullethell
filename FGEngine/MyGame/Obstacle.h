#pragma once
#include "EntityManager.h"
#include "Vector2D.h"
#include <SDL_pixels.h>
#include "Projectile.h"
#include "ProjectilePool.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include <vector>

static constexpr int s_HealthValues[4] = {15,35,50,9999};


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
	Obstacle(FG::EntityManager* eManager, FG::ResourceManager* rManager, FG::Camera* camera);
	void Render(FG::Camera* const camera) override;
	void EnterNextPhase();

	SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;
	bool AddSprite(FG::Sprite* spr) override;

	void StartPosition(FG::Vector2D pos);
	void EnterScreen();
	void PlaceOffscreenForEntrance();
	Phase CurrentPhase();
	void SetUp();
	int GetHealth() { return health; }
	int GetMaxHealth() { return s_HealthValues[static_cast<int>(phase)]; }
private:

	int health = s_HealthValues[0];
	Phase phase = Phase::first;

	float firePauseDuration = .2f;
	float firePauseTime;
	
	float barrageDuration = 2.0f;
	float barrageTime;

	float barragePauseDuration = 1.0f;
	float barragePauseTime;

	bool isColliding = false;
	SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	SDL_Color CollidingColor = { 255, 0, 0, 255 };
	
	FG::EntityManager* entityManager;
	FG::ResourceManager* resourcecManager;
	AudioManager* audioManager;

	FG::Camera* camera = nullptr;
	std::vector<FG::Sprite*> sprites;

	bool entersScreen;
	bool firstBattle = true;

	FG::Vector2D startPosition;

	ProjectilePool* projectilePool = nullptr;

	void DrawBoundingBox();
	void DrawColliderCircle();
	void OnDeath();
	void Fire();
};


