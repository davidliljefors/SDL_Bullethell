#pragma once
#include "Entity.h"
#include "Vector2D.h"
#include <SDL_pixels.h>
#include "Projectile.h"
#include "ProjectilePool.h"

namespace FG
{
	class Sprite;
	class Camera;
}

class Obstacle : public FG::Entity
{
public:
	void Update(float deltaTime) override;
	Obstacle(FG::Camera* camera);
	void Render(FG::Camera* const camera) override;

	SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;

	void StartPosition(FG::Vector2D pos);
	void EnterScreen();
private:
	bool isColliding = false;
	SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	SDL_Color CollidingColor = { 255, 0, 0, 255 };
	FG::Camera* camera = nullptr;

	bool entersScreen;
	FG::Vector2D startPosition;

	ProjectilePool* projectilePool = nullptr;

	void DrawBoundingBox();
	void DrawColliderCircle();
};


