#include "Sensor.h"
#include "Projectile.h"

Sensor::Sensor(Player* p_user, SENSOR_TYPE p_type, float p_radius)
{
	m_user = p_user;
	m_type = p_type;
	AddCircleCollider(p_radius);
}

void Sensor::OnCollision(Entity* other)
{
	switch (m_type)
	{
	case Sensor::graze:
		if (typeid(*other) == typeid(Projectile))
		{
			//Award puntos
			//m_user->
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
