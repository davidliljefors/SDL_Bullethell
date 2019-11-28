#pragma once
#include <vector>

class Projectile;

namespace FG
{
	class Entity;
	class EntityManager;
}

class ProjectilePool
{
public:
	ProjectilePool(int maxBullets, Projectile* projectilePrefab, FG::EntityManager* entityManager);

	~ProjectilePool() { Destroy(); }

	void ReloadAll();
	void PoolProjectile(Projectile* p);
	Projectile* GetProjectile();
	Projectile* GetProjectile(const Projectile& projectile);

	void Destroy();

private:
	int maxBullets;


	Projectile** inactiveProjs;
	std::vector<FG::Entity*> projectiles;
};