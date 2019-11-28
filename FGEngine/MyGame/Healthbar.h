#pragma once
#include <Entity.h>


class Obstacle;

class Healthbar : public FG::Entity {
public:
	Healthbar(FG::Vector2D position, FG::Vector2D size, Obstacle* boss)
		: position(position), size(size), boss(boss)
	{}

	void Update() {};
	void Render(FG::Camera* camera);
	void SetActive(bool state);

private:
	FG::Vector2D position;
	FG::Vector2D size;
	Obstacle* boss;
	bool isActive = false;
};