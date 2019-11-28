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
	Projectile(FG::Sprite* sprite, bool playerFired, FG::Vector2D direction, float speed, FG::Camera* camera, float lifetime = -1, float accelerationSpeed = 0, float rotation = 0);
	Projectile(FG::Sprite* sprite, bool playerFired, FG::Vector2D direction, float speed,
		FG::Camera* camera, ProjectilePool* pool, Projectile* explosionProjectile, int projectileCount, float lifetime = -1, float accelerationSpeed = 0, float rotation = 0);
	Projectile(const Projectile&);
	Projectile& operator=(const Projectile& other);
	~Projectile();

	void Update(float deltaTime) override;
	void Render(FG::Camera* const camera) override;

	virtual SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;
	bool Expired() { return elapsedTime > lifetime; }
	bool Grazed() { return grazed; }
	void Fire(FG::Vector2D firePosition);
	void SetValues(const Projectile& projectile);
	void SetSpeed(float speed);

	bool IgnoreCollision() override;
	void Reload();
	void OnGrazed();

private:
	FG::Camera* camera = nullptr;

	ProjectileType type;

	ProjectilePool* pool = nullptr;
	Projectile* explosionProjectile = nullptr;
	int projectileCount;

	bool isColliding = false;
	bool playerFired;
	bool grazed = false;

	float elapsedTime = 0.f;
	float lifetime;

	FG::Vector2D direction;
	float speed = 1;
	float accelerationSpeed;
	float rotation;
	float angle;

	void ExplodeProjectile();
		
	virtual void OnLifetimeEnd() {}

	constexpr static SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	constexpr static SDL_Color CollidingColor = { 255, 0, 0, 255 };
	
	constexpr static float OFFSCREEN_LIMIT = 50;
	FG::Vector2D maxBoundaries;

	//Comment
};

