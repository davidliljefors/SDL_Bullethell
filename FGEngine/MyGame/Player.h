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
public:
	float speed = 500.0f;
	int lives = 3;

	float fireCooldown = .125f;
	float fireTime;
	
	Player(FG::EntityManager* entityManager, FG::InputManager* inputManager, FG::Camera* camera, FG::Vector2D boundaries);

	void Update(float deltaTime) override;
	void Render(FG::Camera* const camera) override;
	Projectile* projectilePrefab = nullptr;

	virtual SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;

private:
	FG::InputManager* inputManager = nullptr;
	FG::EntityManager* entityManager = nullptr;
	FG::Camera* camera = nullptr;
	bool isColliding = false;
	SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	SDL_Color CollidingColor = { 255, 0, 0, 255 };

	FG::Vector2D minBoundaries;
	FG::Vector2D maxBoundaries;


	Player() {}

	void Shoot();
	void DrawBoundingBox();
	void DrawColliderCircle();
	void MovePlayer(float deltaTime);
	void MoveCamera(float deltaTime);
};
