#pragma once

#include <Entity.h>
#include <vector>
#include <utility>

class Player;

namespace FG
{
	class Sprite;
	class Camera;
}

class Sensor : public FG::Entity
{
public:

	enum SENSOR_TYPE { graze, itemGrab };

	Sensor(Player* p_user, SENSOR_TYPE p_type, float p_radius, FG::Sprite* grazeSprite);
	void Update(float deltaTime);
	void Render(FG::Camera* const camera);
	void OnCollision(Entity* other);

private:
	static constexpr float visualGrazeTime = .15f;
	std::vector<std::pair<FG::Vector2D, float>> grazeLocations;
	FG::Sprite* grazeSprite;
	SENSOR_TYPE m_type;
	Player* user;
};
