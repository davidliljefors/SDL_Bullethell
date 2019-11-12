#include "GameApplication.h"
#include <Window.h>
#include <InputManager.h>
#include <Camera.h>
#include <EntityManager.h>
#include <ResourceManager.h>
#include <Sprite.h>

#include <Logger.h>
#include <SDL.h>

#include "Player.h"
#include "Obstacle.h"

bool GameApplication::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		FG::Logger::Log(SDL_GetError(), FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
		return false;
	}

	window = new FG::Window();
	if (!window->Initialize("My game", 1024, 768))
	{
		FG::Logger::Log(SDL_GetError(), FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
		return false;
	}

	inputManager = new FG::InputManager();
	inputManager->Initialize();

	camera = new FG::Camera();
	if (!camera->Initialize(window))
	{ return false; }

	resourceManager = new FG::ResourceManager();
	FG::Sprite* sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "dodonpachi.png");
	resourceManager->AddResource("dodonpachi.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "hippie.png");
	resourceManager->AddResource("hippie.png", sprite);

	entityManager = new FG::EntityManager();

	Player* player = new Player(inputManager, camera);
	player->sprite = resourceManager->GetResource<FG::Sprite>("dodonpachi.png");
	player->position.x = 500.f;
	player->position.y = 600.f;
	player->AddCircleCollider(32.f);
	entityManager->AddEntity(player);

	Obstacle* obstacle = new Obstacle(camera);
	obstacle->sprite = resourceManager->GetResource<FG::Sprite>("hippie.png");
	obstacle->position.x = 500.f;
	obstacle->position.y = 100.f;
	obstacle->AddCircleCollider(250.f);
	entityManager->AddEntity(obstacle);

	return true;
}

void GameApplication::Run()
{
	bool quit = false;
	while (!quit)
	{
		time.StartFrame();
		inputManager->Update(quit);
		entityManager->Update(time.DeltaTime());
		entityManager->DoCollisions();
		camera->StartRenderFrame();
		entityManager->Render(camera);
		camera->EndRenderFrame();
		time.EndFrame();
		// Wait to achieve target framerate 
		if (time.DeltaTime() < frameDelay)
		{
			SDL_Delay(frameDelay - time.DeltaTime());
		}
		time.EndFrame();
	}
}

void GameApplication::Shutdown()
{
	if (entityManager)
	{
		entityManager->Shutdown();
		delete entityManager;
		entityManager = nullptr;
	}

	if(resourceManager)
	{
		resourceManager->Shutdown();
		delete resourceManager;
		resourceManager = nullptr;
	}

	if (camera)
	{
		camera->Shutdown();
		delete camera;
		camera = nullptr;
	}

	if (inputManager)
	{
		delete inputManager;
		inputManager = nullptr;
	}


	if (window)
	{
		window->Shutdown();
		delete window;
		window = nullptr;
	}

	SDL_Quit();
}
