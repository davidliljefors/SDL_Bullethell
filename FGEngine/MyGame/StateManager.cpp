#include <sstream>
#include <iomanip>
#include <ctime>

#include "StateManager.h"
#include <Text.h>
#include <Sprite.h>
#include <EntityManager.h>
#include <InputManager.h>

#include "Camera.h"
#include "Config.h"

#include "Player.h"
#include "Obstacle.h"

GAME_STATES State::state = GAME_STATES::start;

StateManager::StateManager(FG::EntityManager* eManager, InputManager* iManager, AudioManager* aManager, ResourceManager* rManager, Camera* camera) :
	entityManager(eManager), inputManager(iManager), audioManager(aManager), resourceManager(rManager), camera(camera), screenBoundaries(Config::screenBoundaries)
{
	scoreController = new ScoreController();

	// ENTITIES
	player = new Player({ 500, 650 }, this,
		new Projectile(resourceManager->GetResource<FG::Sprite>("BullethellBullet.png"), true, FG::Vector2D::Down, 1000.f, camera, 5.0f));
	player->AddSprite(resourceManager->GetResource<FG::Sprite>("Bullethellplayer.png"));

	player->AddCircleCollider(player->sprite->size.x / 8.f);
	player->AddColliderSprite(resourceManager->GetResource<FG::Sprite>("playercollider.png"));
	entityManager->AddEntity(player);

	//Boss
	boss = new Obstacle({ 500, 100 }, this);
	boss->AddCircleCollider(64);
	boss->AddSprite(resourceManager->GetResource<FG::Sprite>("BullethellBoss.png"));
	boss->AddSprite(resourceManager->GetResource<FG::Sprite>("BullethellBoss.png"));
	boss->AddSprite(resourceManager->GetResource<FG::Sprite>("BullethellBoss.png"));
	boss->AddSprite(resourceManager->GetResource<FG::Sprite>("BullethellBoss.png"));
	boss->Initialize();
	entityManager->AddEntity(boss);

	//UI ELEMENTS
	bossHPBar = new Healthbar({ screenBoundaries.x/2 - screenBoundaries.x/4, screenBoundaries.y - 30 }, { screenBoundaries.x /2, 10 }, boss);
	entityManager->AddEntity(bossHPBar);

	logo = new Text();
	logo->SetText(camera->GetInternalRenderer(), "Help me irl", "radiospace.ttf", 128, { 250,250,250 });
	resourceManager->AddResource("logo", logo);

	spacePrompt = new Text();
	spacePrompt->SetText(camera->GetInternalRenderer(), "Press SPACE to dead", "radiospace.ttf", 36, { 225,225,225 });
	resourceManager->AddResource("spacePrompt", spacePrompt);

	hiScoreDisplay = new Text();
	hiScoreDisplay->SetText(camera->GetInternalRenderer(), "Press SPACE to dead", "radiospace.ttf", 36, { 225,225,225 });
	resourceManager->AddResource("hiScoreDisplay", hiScoreDisplay);

	playerLives = new Sprite * [player->maxLives];
	liveContainers = new Sprite * [player->maxLives];
	for (size_t i = 0; i < player->maxLives; i++)
	{
		playerLives[i] = resourceManager->GetResource<FG::Sprite>("heart.png");
		liveContainers[i] = resourceManager->GetResource<FG::Sprite>("heartcontainer.png");
	}
	playerBombs = new Sprite * [player->maxBombs];
	bombContainers = new Sprite * [player->maxBombs];
	
	for (size_t i = 0; i < player->maxBombs; i++)
	{
		playerBombs[i] = resourceManager->GetResource<FG::Sprite>("bomb.png");
		bombContainers[i] = resourceManager->GetResource<FG::Sprite>("bombcontainer.png");
	}
	
	currentScoreDisplay = new Text();
	currentScoreDisplay->SetText(camera->GetInternalRenderer(), "0000000000", "radiospace.ttf", 48, { 225,225,225 });
	resourceManager->AddResource("currentScoreDisplay", currentScoreDisplay);

	currentHiScoreDisplay = new Text();
	currentHiScoreDisplay->SetText(camera->GetInternalRenderer(), "HI - 0000000000", "radiospace.ttf", 36, { 225,225,225 });
	resourceManager->AddResource("currentHiScoreDisplay", currentHiScoreDisplay);

	firstBattle = true;
}

StateManager::~StateManager()
{

}

void StateManager::Update()
{
	switch (State::state)
	{
	case start:
		if (inputManager->IsKeyDown(SDL_SCANCODE_SPACE)) {
			State::state = game;
			player->OnStartBattle();
			boss->EnterScreen();
		}
		break;
	case game:

		int currentDisplayScore;
		currentDisplayScore = scoreController->Update();

		if (lastDisplayScore != currentDisplayScore) {
			lastDisplayScore = currentDisplayScore;
			
			std::stringstream s;

			s << std::setw(10) << std::setfill('0') << lastDisplayScore;
			currentScoreDisplay->SetText(camera->GetInternalRenderer(), s.str(), "radiospace.ttf", 48, { 225,225,225 });
		}
		/*
		if (inputManager->IsKeyDown(SDL_SCANCODE_Z)) {
			scoreController->AddScore(100);
		}
		*/
		if (boss->CurrentPhase() == Phase::dead) {
			//boss->Reset();
			player->OnVictory();
			State::state = start;
			scoreController->ResetScore();
		}
		break;
	default:
		break;
	}
}

void StateManager::Render(Camera* const camera)
{
	switch (State::state)
	{
	case start:
		logo->Render(camera, { screenBoundaries.x / 2, screenBoundaries.y / 2 });
		spacePrompt->Render(camera, { screenBoundaries.x / 2, screenBoundaries.y *.75f });
		break;
	case game:
		
		currentScoreDisplay->Render(camera, { screenBoundaries.x * .84f, screenBoundaries.y * .09f });
		
		currentHiScoreDisplay->Render(camera, { screenBoundaries.x * .84f, screenBoundaries.y * .029f});
		for (size_t i = 0; i < player->maxLives; i++)
		{
			if (i < player->CurrentLives())
				playerLives[i]->Render(camera, { screenBoundaries.x - (playerLives[i]->size.x / 2) * 2,  (screenBoundaries.y * .11f) + (playerLives[i]->size.y / 2) * (2 * (i + 1)) });
			liveContainers[i]->Render(camera, { screenBoundaries.x - (liveContainers[i]->size.x /2) * 2,  (screenBoundaries.y * .11f) + (liveContainers[i]->size.y /2 )*(2*(i+1)) });
		}
		for (size_t i = 0; i < player->maxBombs; i++)
		{
			if (i < player->CurrentBombs())
				playerBombs[i]->Render(camera, { screenBoundaries.x - (playerBombs[i]->size.x / 2) * 4.5f,  (screenBoundaries.y * .11f) + (playerBombs[i]->size.y / 2) * (2 * (i + 1)) });
			bombContainers[i]->Render(camera, { screenBoundaries.x - (bombContainers[i]->size.x / 2) * 4.5f,  (screenBoundaries.y * .11f) + (bombContainers[i]->size.y / 2) * (2 * (i + 1)) });
		}
		break;
	default:
		break;
	}
			
}
