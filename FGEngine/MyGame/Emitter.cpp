#include "Emitter.h"
#include "ProjectilePool.h"

Emitter::Emitter(FG::Vector2D position, ProjectilePool* pool, StateManager* stateManager, float lifeTime, float angle)
	: entityManager(stateManager->entityManager), resourceManager(stateManager->resourceManager), camera(stateManager->camera),
	position(position), projectilePool(pool), angle(angle)
{
	originalAngle = angle;
}
/*
void Emitter::SetEmitter(Projectile& proj, int amount, float minAngle, float maxAngle)
{
	projectile = &proj;
}
*/

void Emitter::SetEmitter(EmitterProperties* emitterProperties)
{
	properties = emitterProperties;
	ResetTime();
	projectile = properties->projectile;
}

void Emitter::Move(FG::Vector2D position)
{
	this->position = position;
}

void Emitter::SetAngle(float newAngle)
{
	angle = newAngle;
}

void Emitter::ResetTime()
{
	barrageTime = properties->barrageDuration;
	firePauseTime = properties->firePauseDuration;
	barragePauseTime = properties->barragePauseDuration/2;
	angle = originalAngle;
}
/*
void Emitter::Fire(int amount, float minAngle, float maxAngle)
{
	float currentAngle;

	if (amount == 1)
	{
		Projectile* proj = projectilePool->GetProjectile(*projectile);

		proj->SetDirection(FG::Vector2D::AngleToVector2D(angle));

		proj->Fire(position);
	}
	else
	{
		for (size_t i = 0; i < amount; i++)
		{
			currentAngle = minAngle + ((float)i / (amount - 1) * (maxAngle - minAngle));

			Projectile* proj = projectilePool->GetProjectile(*projectile);

			proj->SetDirection(FG::Vector2D::AngleToVector2D(currentAngle + angle));

			proj->Fire(position);
		}
	}
}
*/
#include <iostream>

void Emitter::Fire(float deltaTime, FG::Vector2D targetPosition)
{
	if (barragePauseTime <= 0) {
		if (barrageTime > 0) {
			if (firePauseTime > 0) {
				firePauseTime -= deltaTime;
				if (firePauseTime <= 0) {

					if (properties->aimAtPlayer)
						angle = (targetPosition - position).GetAngle();
					else
						angle += (properties->spinning ? properties->spinSpeed : 0);
					//angle *= (properties->spinning ? barrageTime / properties->barrageDuration : 1);

				//std::cout << angle << std::endl;

					SetAngle(angle);

					float currentAngle;

					if (properties->bulletsAtOnce == 1)
					{
						Projectile* proj = projectilePool->GetProjectile(*projectile);

						proj->SetDirection(FG::Vector2D::AngleToVector2D(angle));

						proj->Fire(position);
					}
					else
					{
						for (size_t i = 0; i < properties->bulletsAtOnce; i++)
						{
							currentAngle = properties->minOffsetAngle + ((float)i / (properties->bulletsAtOnce - 1) * (properties->maxOffsetAngle - properties->minOffsetAngle));

							Projectile* proj = projectilePool->GetProjectile(*projectile);

							proj->SetDirection(FG::Vector2D::AngleToVector2D(currentAngle + angle));

							proj->Fire(position + firePosOffset);
						}
					}
				}
			}
			else
				firePauseTime = properties->firePauseDuration;
		}
		barrageTime -= deltaTime;
		if (barrageTime <= 0)
		{
			barrageTime = properties->barrageDuration;
			barragePauseTime = properties->barragePauseDuration;
			firePauseTime = properties->firePauseDuration;
		}
	}
	else {
		barragePauseTime -= deltaTime;
	}
}
