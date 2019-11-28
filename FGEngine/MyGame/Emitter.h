#pragma once
#include <Entity.h>

#include "Projectile.h"
#include "ProjectilePool.h"
#include "StateManager.h"

namespace FG
{
	class Sprite;
	class Camera;
}

enum FireModes
{
	Linear,
	Burst
};

class Emitter : public FG::Entity
{
public:
	Emitter(FG::Vector2D position, ProjectilePool* pool, StateManager* stateManager, float lifeTime = -1, float angle = 270);

	void Move(FG::Vector2D position);
	void SetAngle(float newAngle);
	bool Fire(const Projectile& projectile, float deltaTime, int amount = 1, float minAngle = 0, float maxAngle = 360, int waves = 1, float timeBetweenWaves = 0, FireModes fireMode = Linear);

private:
	Sprite* sprite = nullptr;
	Camera* camera = nullptr;

	ProjectilePool* projectilePool = nullptr;
	FG::EntityManager* entityManager = nullptr;
	FG::ResourceManager* resourceManager = nullptr;

	FG::Vector2D position;
	float angle = 0;

	float elapsedTime = 0;
	float projectilesFired = 0;
};

