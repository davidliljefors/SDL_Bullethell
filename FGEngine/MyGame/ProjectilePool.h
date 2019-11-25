#pragma once

#include <array>

class Projectile;

namespace FG
{
	class EntityManager;
}

class ProjectilePool
{
public:
	ProjectilePool(int maxBullets, Projectile* projectilePrefab, FG::EntityManager* entityManager);

	~ProjectilePool() { Destroy(); }

	Projectile* GetProjectile();
	Projectile* GetProjectile(const Projectile& projectile);

	void Destroy();

private:
	int maxBullets;
	Projectile** projectiles;
};