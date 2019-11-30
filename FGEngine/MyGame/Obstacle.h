#pragma once
#include "StateManager.h"
#include "Vector2D.h"
#include <SDL_pixels.h>
#include "Projectile.h"
#include <vector>
#include "Explosion.h"
#include "Emitter.h"

static constexpr int s_HealthValues[4] = { 30,50,75,9999 };

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

struct BossPhase {

public:
	int health;

	Sprite* sprite;

	float barragePauseDuration;

	std::vector<FG::Vector2D> positions;
	bool moveAfterBarrage;
	float movePauseDuration;

	std::vector<EmitterProperties*> emitters;
	
	BossPhase(std::vector<EmitterProperties*> emitters, FG::Vector2D initialPos, int health = 35, float barragePauseDuration = 2.0f, bool moveAfterBarrage = true, float movePauseDuration = 1.0f)
	: emitters(emitters), health(health), barragePauseDuration(barragePauseDuration), moveAfterBarrage(moveAfterBarrage), movePauseDuration(movePauseDuration)
	{
		positions.push_back(initialPos);
	};

	void AddPosition(FG::Vector2D pos)
	{
		positions.push_back(pos);
	};
};

class Obstacle : public FG::Entity
{
public:
	void Initialize();
	void Update(float deltaTime) override;
	Obstacle(FG::Vector2D pos, StateManager* stateManager);
	void Render(FG::Camera* const camera) override;
	void EnterNextPhase();

	SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;
	bool IgnoreCollision() override;
	bool AddSprite(FG::Sprite* spr) override;
	void SetIndicatorSprite(FG::Sprite* spr) { bottomIndicator = spr; }

	void EnterScreen();
	void PlaceOffscreenForEntrance();
	int GetCurrentPhase();
	int GetPhaseCount();
	Phase CurrentPhase(){ return phase; }
	void SetUp();
	int GetHealth() { return health; }
	//int GetMaxHealth() { return s_HealthValues[static_cast<int>(phase)]; }
	int GetMaxHealth() { return bossPhases[currentBossPhase]->health; }
	bool Invincible() { return invincibleTime < invincibleDuration; }
private:

	int health = s_HealthValues[0];
	int currentMaxHealth = s_HealthValues[0];
	Phase phase = Phase::first;

	//static constexpr float firePauseDuration = .025f;
	//static constexpr float barrageDuration = 3.0f;
	//static constexpr float barragePauseDuration = 2.0f;

	static constexpr float invincibleDuration = 1.0f;
	static constexpr float invincibleAlphaBlinkDuration = .1f;

	//float firePauseTime;
	//float barrageTime;
	float barragePauseTime;
	float movePauseTime;

	float invincibleTime;
	float invincibleAlphaBlinkTime;
	bool invincibleAlphaBlink;
	static constexpr float hitFlashDuration = 0.07f;
	float currentHitFlash = 0;

	//std::vector<std::vector<FG::Vector2D>> bossPositions;

	bool isColliding = false;
	static constexpr SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	static constexpr SDL_Color CollidingColor = { 255, 0, 0, 255 };

	FG::EntityManager* entityManager;
	FG::ResourceManager* resourcecManager;
	AudioManager* audioManager;
	ScoreController* scoreController;

	FG::Camera* camera = nullptr;
	std::vector<FG::Sprite*> sprites;
	FG::Sprite* bottomIndicator = nullptr;
	float indicatorYOffset = 760;

	bool entersScreen;
	bool firstBattle = true;

	FG::Vector2D startPosition;
	FG::Vector2D destination;

	Explosion* explosion;

	Player* player;

	std::vector<BossPhase*> bossPhases;
	int currentBossPhase = 0;

	Emitter* emitter;
	std::vector<Emitter*> emitters;

	ProjectilePool* projectilePool = nullptr;
	
	void StartPosition(FG::Vector2D pos);
	void OnDeath();

	void ResetEmittersTime();
	void Fire(Projectile projectile, float angle = 270, bool targetPlayer = false, int bullets = 1, float minAngle = 0, float maxAngle = 360);

	void MoveToAnotherPosition();

};


