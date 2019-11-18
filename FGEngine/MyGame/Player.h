#pragma once
#include <array>

#include <Entity.h>
#include <Vector2D.h>
#include "Projectile.h"



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
	int maxLives = 3;

	Player(FG::EntityManager* entityManager, FG::InputManager* inputManager, FG::Camera* camera, FG::Vector2D boundaries, Projectile* projectile);
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
	void EnterScreen();

private:
	FG::InputManager* inputManager = nullptr;
	FG::EntityManager* entityManager = nullptr;
	FG::Camera* camera = nullptr;
	FG::Sprite* colliderSprite = nullptr;
	bool isColliding = false;
	static constexpr SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	static constexpr SDL_Color CollidingColor = { 255, 0, 0, 255 };

	FG::Vector2D minBoundaries;
	FG::Vector2D maxBoundaries;

	FG::Vector2D startPosition;

	int lives;

	bool entersScreen = false;

	float fireCooldown = .125f;
	float fireTime;

	float respawnPauseDuration = 1.0f;
	float respawnPauseTime;

	float invincibleDuration = 3.0f;
	float invincibleTime;

	float invincibleAlphaBlinkDuration = .1f;
	float invincibleAlphaBlinkTime;
	bool invincibleAlphaBlink;

	static constexpr int MAX_BULLETS = 50;
	Projectile* projectiles[MAX_BULLETS];
	
	void SetUp();
	void Respawn();
	void Shoot();
	void DrawBoundingBox();
	void DrawColliderCircle();
	void MovePlayer(float deltaTime);
	void MoveCamera(float deltaTime);

};
