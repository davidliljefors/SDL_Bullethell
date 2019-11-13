#pragma once

#include <Entity.h>
#include "Player.h"

class Sensor : public FG::Entity
{
public:

	enum SENSOR_TYPE { graze, itemGrab };

	Sensor(Player* p_user, SENSOR_TYPE p_type, float p_radius);
	void OnCollision(Entity* other);

private:

	SENSOR_TYPE m_type;
	Player* m_user;
};
