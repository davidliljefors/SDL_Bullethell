#pragma once

#include <Entity.h>
#include <Vector2D.h>
#include "ProjectilePool.h"

namespace FG
{
	class Window;
	class Camera;
	class Sprite;
	class EntityManager;
}

enum ProjectileType
{
	Regular,
	Exploding
};

class Projectile : public FG::Entity
{
public:
	Projectile(FG::Sprite* sprite, float lifetime, bool playerFired, FG::Vector2D velocity, float accelerationSpeed,
		FG::Camera* camera, FG::Vector2D boundaries);
	Projectile(FG::Sprite* sprite, float lifetime, bool playerFired, FG::Vector2D velocity, float accelerationSpeed,
		FG::Camera* camera, FG::Vector2D boundaries, ProjectilePool* pool, Projectile* explosionProjectile, int projectileCount);
	Projectile(const Projectile&);
	~Projectile();

	void Update(float deltaTime) override;
	void Render(FG::Camera* const camera) override;

	virtual SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;
	bool Expired() { return elapsedTime > lifetime; }
	void Fire(FG::Vector2D firePosition);
	void SetValues(const Projectile& projectile);

	bool IgnoreCollision() override;

private:
	FG::Camera* camera = nullptr;

	ProjectileType type;

	ProjectilePool* pool = nullptr;
	Projectile* explosionProjectile = nullptr;
	int projectileCount;

	bool isColliding = false;
	bool playerFired;

	float elapsedTime = 0.f;
	float lifetime;

	FG::Vector2D velocity;
	float speedMult = 1;
	float accelerationSpeed;


	void DrawColliderCircle();
	void DrawBoundingBox();
	void ExplodeProjectile();

	void Reload();
	virtual void OnLifetimeEnd() {}

	constexpr static SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	constexpr static SDL_Color CollidingColor = { 255, 0, 0, 255 };
	
	constexpr static float OFFSCREEN_LIMIT = 50;
	FG::Vector2D maxBoundaries;
};

