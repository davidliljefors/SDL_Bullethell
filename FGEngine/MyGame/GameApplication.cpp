#include "GameApplication.h"

#include <Logger.h>
#include <Window.h>
#include <InputManager.h>
#include <Camera.h>
#include <EntityManager.h>

#include "Player.h"
#include "Enemy.h"

#include <SDL.h>

bool GameApplication::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		FG::Logger::Log(SDL_GetError(), FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
		return false;
	}

	window = new FG::Window();
	if (!window->Initialize("My Game", 1024, 768, false))
	{
		FG::Logger::Log("Error: Failed to initialize window.", FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
		return false;
	}

	inputManager = new FG::InputManager();
	inputManager->Initialize();

	camera = new FG::Camera();
	camera->Initialize(window);

	entityManager = new FG::EntityManager();

	Player* p;
	Enemy* e;
	entityManager->AddEntity(p = new Player(inputManager, camera));
	entityManager->AddEntity(e = new Enemy(camera, FG::Vector2D(150, 250)));
	entityManager->AddEntity(new Enemy(camera, FG::Vector2D(450, 250)));
	e->collider.SetSize(25);
	p->collider.SetSize(25);

	return true;
}

void GameApplication::Run()
{
	while (!quit)
	{
		time.StartFrame();

		inputManager->Update(quit);
		entityManager->Update(time.DeltaTime());
		camera->StartRenderFrame();
		entityManager->Render(camera);
		camera->EndRenderFrame();
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
