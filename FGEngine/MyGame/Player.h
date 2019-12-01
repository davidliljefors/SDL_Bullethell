#pragma once
#include <array>

#include "StateManager.h"
#include <Entity.h>
#include <Vector2D.h>
#include "Projectile.h"
#include "ProjectilePool.h"
#include "Bomb.h"
#include "Sensor.h"
#include "Explosion.h"

namespace FG
{
	class Window;
	class InputManager;
	class EntityManager;
	class Camera;
	class Sprite;
}

class Player : public FG::Entity
{
	static constexpr float focusMultiplier = 0.5f;
	
public:
	static constexpr float speed = 500.0f;
	static constexpr int maxLives = 5;
	static constexpr int maxBombs = 3;

	Player(FG::Vector2D pos, StateManager* stateManager, Projectile* projectile);
	~Player();

	bool Respawning() { return respawnPauseTime > 0; }
	bool Invincible() { return invincibleTime < invincibleDuration; }

	void Update(float deltaTime) override;
	void Render(FG::Camera* const camera) override;
	bool AddSprite(FG::Sprite* sprite) override;
	Projectile* projectilePrefab = nullptr;

	virtual SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;
	void AddColliderSprite(FG::Sprite* sprite) { this->colliderSprite = sprite; }
	bool IgnoreCollision() override;

	void OnVictory();
	void OnStartBattle();
	bool OnGraze();

	int CurrentLives();
	int CurrentBombs();
	

	void AwardReward();

	//todo private
	FG::Sprite* leftSprite = nullptr;
	FG::Sprite* straightSprite = nullptr;
	FG::Sprite* rightSprite = nullptr;
private:
	Bomb* bomb = nullptr;
	FG::InputManager* inputManager = nullptr;
	FG::EntityManager* entityManager = nullptr;
	AudioManager* audioManager = nullptr;
	ScoreController* scoreController = nullptr;
	

	FG::Camera* camera = nullptr;
	FG::Sprite* colliderSprite = nullptr;
	Sensor* sensor = nullptr;
	bool isColliding = false;
	static constexpr SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	static constexpr SDL_Color CollidingColor = { 255, 0, 0, 255 };

	FG::Vector2D minBoundaries;
	FG::Vector2D maxBoundaries;

	FG::Vector2D startPosition;

	int lives;
	int bombs;
	bool rewardPlayer = true;

	bool entersScreen = false;
	bool firstBattle = true;

	ProjectilePool* projectilePool = nullptr;

	static constexpr float fireCooldown = .125f;
	static constexpr float respawnPauseDuration = 1.0f;
	static constexpr float invincibleDuration = 3.0f;
	static constexpr float invincibleAlphaBlinkDuration = .1f;
	static constexpr float counterbombTimeframe = 0.3f;

	float fireTime;
	float respawnPauseTime;
	float invincibleTime;
	float invincibleAlphaBlinkTime;
	float timeSincelastHit = 0;

	bool invincibleAlphaBlink;
	bool hit = false;
	static constexpr float hitFlashDuration = 0.02f;
	float hitFlashElapsedTime = 0.f;
	bool hitFlash = false; // used to change between flash colour during slowmo


	Explosion* explosion;

	static constexpr int MAX_BULLETS = 50;
	bool godMode = false;
	void StartPosition(FG::Vector2D pos);
	void SetUp();
	void Respawn();
	void EnterScreen();
	void PlaceOffscreenForEntrance();
	void GetHit();

	void Shoot();
	void MovePlayer(float deltaTime);

};
