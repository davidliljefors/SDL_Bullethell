#pragma once

#include <Entity.h>
class Player;

namespace FG
{
	class Camera;
}

class Sensor : public FG::Entity
{
public:

	enum SENSOR_TYPE { graze, itemGrab };

	Sensor(Player* p_user, SENSOR_TYPE p_type, float p_radius);
	void Update(float deltaTime);
	void Render(FG::Camera* const camera);
	void OnCollision(Entity* other);

private:

	SENSOR_TYPE m_type;
	Player* user;
};
