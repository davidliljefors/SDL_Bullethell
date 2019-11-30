#include "ProjectilePool.h"
#include "Projectile.h"
#include "EntityManager.h"

ProjectilePool::ProjectilePool(int maxBullets, Projectile* projectilePrefab, FG::EntityManager* entityManager) : maxBullets(maxBullets)
{
	inactiveProjs = new Projectile * [maxBullets];

	for (int i = 0; i < maxBullets; i++)
	{
		Projectile* p = new Projectile(*projectilePrefab);
		p->SetIndex(i);
		p->SetPool(this);
		inactiveProjs[i] = p;
		//projectiles[i] = nullptr;
		//entityManager->AddEntity(projectiles[i]);
	}
	entityManager->AddPool(&projectiles);
}

void ProjectilePool::ReloadAll()
{
	for (auto it = projectiles.begin(); it != projectiles.end(); it++)
	{
		auto p = static_cast<Projectile*>(*it);
		inactiveProjs[p->GetIndex()] = p;
	}
	projectiles.clear();
}
void ProjectilePool::PoolProjectile(Projectile* p)
{
	int indexInVector = -1;
	for (int i = 0; i < projectiles.size(); i++)
	{
		if (projectiles[i] == p)
		{
			indexInVector = i;
			inactiveProjs[p->GetIndex()] = p;
			break;
		}
	}
	if (indexInVector != -1)
	{
		projectiles.erase(projectiles.begin() + indexInVector);
	}
}

Projectile* ProjectilePool::GetProjectile()
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (inactiveProjs[i] != nullptr)
		{
			Projectile* p = inactiveProjs[i];
			projectiles.push_back(inactiveProjs[i]);
			inactiveProjs[i] = nullptr;
			return p;
		}
	}
	throw "out of projectiles";
	return nullptr;
}

Projectile* ProjectilePool::GetProjectile(const Projectile& projectile)
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (inactiveProjs[i] != nullptr)
		{
			Projectile* p = inactiveProjs[i];
			p->SetValues(projectile);
			p->SetPool(this);
			projectiles.push_back(inactiveProjs[i]);
			inactiveProjs[i] = nullptr;
			return p;
		}
	}
	throw "out of projectiles";
	return nullptr;
}

void ProjectilePool::Destroy()
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (inactiveProjs[i])
		{
			delete inactiveProjs[i];
		}
	}
	inactiveProjs = nullptr;
}
