#include "Sensor.h"
#include <Camera.h>
#include <SDL_render.h>
#include "Projectile.h"
#include "Player.h"


Sensor::Sensor(Player* p_user, SENSOR_TYPE p_type, float p_radius, Sprite* grazeSprite)
{
	user = p_user;
	m_type = p_type;
	layer = EntityLayer::Character;
	AddCircleCollider(p_radius);
	collisionLayer.set(6);
	this->grazeSprite = grazeSprite;
}

void Sensor::Update(float deltaTime)
{
	position = user->position;
	for (auto& pair : grazeLocations)
	{
		pair.second -= deltaTime;
	}

	grazeLocations.erase(std::remove_if(grazeLocations.begin(), grazeLocations.end(), [](std::pair<FG::Vector2D, float> pair)
		{
			return pair.second < 0.f;
		}
	), grazeLocations.end());

}

void Sensor::Render(FG::Camera* const camera)
{
	for (auto& pair : grazeLocations)
	{
		//grazeSprite->Render(camera, pair.first, 128, nullptr, -5 * (pair.second - 2.0f));
		grazeSprite->Render(camera, pair.first, 64, nullptr, -1 * (pair.second*4 - 2.0f));
	}
#ifdef _DEBUG
	collider->Draw(camera, 0, 255, 255);
#endif _DEBUG
	}

void Sensor::OnCollision(Entity* other)
{
	switch (m_type)
	{
	case Sensor::graze:
		if (typeid(*other) == typeid(Projectile))
		{
			Projectile* sensedProjectile = static_cast<Projectile*>(other);
			if (!sensedProjectile->Grazed() && user->OnGraze()) {
				sensedProjectile->OnGrazed();
				grazeLocations.emplace_back(std::make_pair(sensedProjectile->position, visualGrazeTime));
			}
		}
		break;
	case Sensor::itemGrab:
		/*if (typeid(*other) == typeid(Item))
		{

		}*/
		break;
	default:
		break;
	}
}
