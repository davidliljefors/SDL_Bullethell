#pragma once
#include <array>

#include <Entity.h>
#include <Vector2D.h>
#include "Projectile.h"
#include "ProjectilePool.h"
#include "AudioManager.h"
#include "Bomb.h"

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
	float speed = 500.0f;
	int maxLives = 5;
	int maxBombs = 3;

	Player(FG::EntityManager* entityManager, FG::InputManager* inputManager, AudioManager* audioManager, FG::Camera* camera, Projectile* projectile);
	~Player();

	bool Respawning() { return respawnPauseTime > 0; }
	bool Invincible() { return invincibleTime < invincibleDuration; }

	void Update(float deltaTime) override;
	void Render(FG::Camera* const camera) override;
	Projectile* projectilePrefab = nullptr;

	virtual SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;
	void AddColliderSprite(FG::Sprite* sprite) { this->colliderSprite = sprite; }
	bool IgnoreCollision() override;

	void StartPosition(FG::Vector2D pos);
	void OnVictory();
	void OnStartBattle();

	int CurrentLives();
	int CurrentBombs();
private:
	Bomb* bomb = nullptr;
	FG::InputManager* inputManager = nullptr;
	FG::EntityManager* entityManager = nullptr;
	AudioManager* audioManager = nullptr;

	FG::Camera* camera = nullptr;
	FG::Sprite* colliderSprite = nullptr;
	bool isColliding = false;
	static constexpr SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	static constexpr SDL_Color CollidingColor = { 255, 0, 0, 255 };

	FG::Vector2D minBoundaries;
	FG::Vector2D maxBoundaries;

	FG::Vector2D startPosition;

	int lives;
	int bombs;

	bool entersScreen = false;
	bool firstBattle = true;

	ProjectilePool* projectilePool = nullptr;

	float fireCooldown = .125f;
	float fireTime;

	float respawnPauseDuration = 1.0f;
	float respawnPauseTime;

	float invincibleDuration = 3.0f;
	float invincibleTime;

	float invincibleAlphaBlinkDuration = .1f;
	float invincibleAlphaBlinkTime;
	bool invincibleAlphaBlink;
	float timeSincelastHit = 0;
	static constexpr float counterbombTimeframe = 0.3f;
	bool hit = false;

	static constexpr int MAX_BULLETS = 50;
	
	void SetUp();
	void Respawn();
	void EnterScreen();
	void PlaceOffscreenForEntrance();
	void GetHit();

	void Shoot();
	void DrawBoundingBox();
	void DrawColliderCircle();
	void MovePlayer(float deltaTime);
	void MoveCamera(float deltaTime);

};
