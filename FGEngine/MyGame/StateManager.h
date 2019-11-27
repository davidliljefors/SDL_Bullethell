#pragma once

#include "GameState.h"
#include "Player.h"
#include "Obstacle.h"
#include "AudioManager.h"
#include "Healthbar.h"
#include "ScoreController.h"

namespace FG
{
	class EntityManager;
	class InputManager;
	class ResourceManager;
	class Text;
	class Sprite;
}
using namespace FG;

class StateManager
{
public:

	StateManager(FG::EntityManager* eManager, InputManager* iManager, AudioManager* aManager, ResourceManager* rManager, Camera* camera);
	~StateManager();

	void Update();
	void Render(Camera* const camera);

private:

	EntityManager* entityManager;
	Vector2D screenBoundaries;
	InputManager* inputManager;
	AudioManager* audioManager;
	ResourceManager* resourceManager;
	ScoreController* scoreController;
	Camera* camera;

	Player* player;
	Obstacle* boss;

	Healthbar* bossHPBar;
	Text* logo;
	Text* spacePrompt;
	Text* hiScoreDisplay;

	Sprite** playerLives;
	Sprite** liveContainers;
	Sprite** playerBombs;
	Sprite** bombContainers;
	Text* currentScoreDisplay;
	Text* currentHiScoreDisplay;

	bool firstBattle;
	int lastDisplayScore;
};