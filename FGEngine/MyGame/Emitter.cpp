#include "Emitter.h"
#include "ProjectilePool.h"

Emitter::Emitter(FG::Vector2D position, ProjectilePool* pool, StateManager* stateManager, float lifeTime, float angle)
	: entityManager(stateManager->entityManager), resourceManager(stateManager->resourceManager), camera(stateManager->camera), position(position), angle(angle)
{
	projectilePool = pool;//new ProjectilePool(1000, 
		//new Projectile(resourceManager->GetResource<FG::Sprite>("bullet.png"), false, FG::Vector2D::Up, 1000.f, camera), entityManager);
}

void Emitter::Move(FG::Vector2D position)
{
	this->position = position;
}

void Emitter::SetAngle(float newAngle)
{
	angle = newAngle;
}

void Emitter::Fire(const Projectile& projectile, int amount, float minAngle, float maxAngle)
{
	float currentAngle;

	for (size_t i = 0; i <= amount; i++)
	{
		currentAngle = minAngle + ((float)i / amount * (maxAngle - minAngle));

		Projectile* proj = projectilePool->GetProjectile(projectile);

		proj->SetDirection(FG::Vector2D::AngleToVector2D(currentAngle + angle));

		proj->Fire(position);
	}
}
