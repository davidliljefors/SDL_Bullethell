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


struct EmitterProperties {

public:
	float firePauseDuration;
	float barrageDuration;
	float barragePauseDuration;
	int bulletsAtOnce;

	float angle;
	float minOffsetAngle;
	float maxOffsetAngle;

	Projectile* projectile;
	bool spinning;
	float spinSpeed;
	bool aimAtPlayer;

	EmitterProperties(Projectile& projectile, float firePause = 0.2f, float barrage = 2.0f, float barragePause = .5f, int bulletsAtOnce = 1, float angle = 270, float minOffsetAngle = 0, float maxOffsetAngle = 360,
		bool aimAtPlayer = false, bool spinning = false, float spinSpeed = 1 )
		: projectile(&projectile), firePauseDuration(firePause), barrageDuration(barrage), barragePauseDuration(barragePause), bulletsAtOnce(bulletsAtOnce), angle(angle),
		minOffsetAngle(minOffsetAngle), maxOffsetAngle(maxOffsetAngle), spinning(spinning), spinSpeed(spinSpeed), aimAtPlayer(aimAtPlayer)
	{}
};

class Emitter : public FG::Entity
{
public:
	Emitter(FG::Vector2D position, ProjectilePool* pool, StateManager* stateManager, float lifeTime = -1, float angle = 270);

	//void SetEmitter(Projectile& proj, int amount, float minAngle = 0, float maxAngle = 360);
	void SetEmitter(EmitterProperties* emitterProperties);
	void Move(FG::Vector2D position);
	void SetAngle(float newAngle);
	void ResetTime();

	bool Fire(float deltaTime, FG::Vector2D targetPosition);
	bool FinishedBarrage() { return barrageTime <= 0; };

private:
	Sprite* sprite = nullptr;
	Camera* camera = nullptr;

	ProjectilePool* projectilePool = nullptr;
	FG::EntityManager* entityManager = nullptr;
	FG::ResourceManager* resourceManager = nullptr;

	FG::Vector2D position;
	FG::Vector2D firePosOffset = FG::Vector2D::Down * 20;
	float angle = 0;
	float originalAngle = 0;

	float firePauseTime;
	float barrageTime;
	float barragePauseTime;
	
	EmitterProperties* properties;

	Projectile* projectile;
};

