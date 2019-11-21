#include "ProjectilePool.h"
#include "Projectile.h"
#include "EntityManager.h"

ProjectilePool::ProjectilePool(int maxBullets, Projectile* projectilePrefab, FG::EntityManager* entityManager): maxBullets(maxBullets)
{
	projectiles = new Projectile*[maxBullets];
	
	for (int i = 0; i < maxBullets; i++)
	{
		projectiles[i] = new Projectile(*projectilePrefab);
		entityManager->AddEntity(projectiles[i]);
	}
}

Projectile* ProjectilePool::GetProjectile()
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (projectiles[i]->Dead())
		{
			return projectiles[i];
		}
	}
	return nullptr;
}

Projectile* ProjectilePool::GetProjectile(Projectile projectile)
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (projectiles[i]->Dead())
		{
			projectiles[i]->SetValues(projectile);
			return projectiles[i];
		}
	}
}

void ProjectilePool::Destroy()
{
	for (int i = 0; i < maxBullets; i++)
	{
		//delete projectiles[i];
		projectiles[i] = nullptr;
	}

	projectiles = nullptr;
}
