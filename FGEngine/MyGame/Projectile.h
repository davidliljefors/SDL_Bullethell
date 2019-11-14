#pragma once

#include <Entity.h>
#include <EntityManager.h>
#include <Vector2D.h>

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
	Exploding,
	Turning
};


class Projectile : public FG::Entity
{
public:
	//Regular Projectile
	Projectile(FG::Sprite* sprite, float lifetime, bool playerFired, FG::Vector2D velocity, float accelerationSpeed,
		FG::Camera* camera, FG::Vector2D boundaries);

	//Exploding Projectile (pointless if you don't implement a lifetime)
	Projectile(FG::Sprite* sprite, float lifetime, bool playerFired, FG::Vector2D velocity, float accelerationSpeed,
		FG::Camera* camera, FG::Vector2D boundaries, FG::EntityManager* entityManger, Projectile* explosionProjectile, int projectileCount);

	//Copy
	Projectile(const Projectile&);

	void Update(float deltaTime) override;
	void Render(FG::Camera* const camera) override;

	virtual SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;
	bool Expired() { return elapsedTime > lifetime; }
	bool Dead() { return dead; }
	void Fire(FG::Vector2D firePosition);

	bool IgnoreCollision() override;

private:
	FG::Camera* camera = nullptr;

	ProjectileType type;
	FG::EntityManager* entityManager;
	Projectile* explosionProjectile;
	float projectileCount;
	

	bool isColliding = false;
	bool playerFired;

	float elapsedTime = 0.f;
	float lifetime;
	FG::Vector2D velocity;
	float speedMult = 1;
	float accelerationSpeed;

	void DrawColliderCircle();
	void DrawBoundingBox();
	void SpawnProjectileExplosion();
	void Move(float deltaTime);

	void Reload();

	SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	SDL_Color CollidingColor = { 255, 0, 0, 255 };
	
	const float OFFSCREEN_LIMIT = 50;
	FG::Vector2D maxBoundaries;

	bool dead;
};

