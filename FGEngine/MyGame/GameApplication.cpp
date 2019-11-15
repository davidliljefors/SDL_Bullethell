#include <iostream>

#pragma region memestuff
// Overloading new and delete to keep track of memory leakage
static int alloc = 0;
void* operator new(std::size_t size)
{
	alloc += 1;
	std::cout << "Alloc : " << size << "bytes, count:" << alloc << std::endl;
	return malloc(size);
}
void operator delete(void* p)
{
	alloc -= 1;;
	std::cout  << "Free, count:"<< alloc << std::endl;
	free(p);
}
#pragma endregion memestuff

#include <SDL.h>
#include <Logger.h>
#include <Window.h>
#include <InputManager.h>
#include <Camera.h>
#include <EntityManager.h>
#include <ResourceManager.h>
#include <Sprite.h>


#include "GameApplication.h"
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
	if (!window->Initialize("My game", SCREENWIDTH, SCREENHEIGHT))
	{
		FG::Logger::Log(SDL_GetError(), FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
		return false;
	}

	inputManager = new FG::InputManager();
	inputManager->Initialize();

	camera = new FG::Camera();
	if (!camera->Initialize(window))
	{
		return false;
	}

	resourceManager = new FG::ResourceManager();
	FG::Sprite* sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "dodonpachi.png");
	resourceManager->AddResource("dodonpachi.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "hippie.png");
	resourceManager->AddResource("hippie.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "bullet.png");
	resourceManager->AddResource("bullet.png", sprite);

	entityManager = new FG::EntityManager();

	Player* player = new Player(entityManager, inputManager, camera, {(float)SCREENWIDTH, (float)SCREENHEIGHT},
		new Projectile(resourceManager->GetResource<FG::Sprite>("bullet.png"), 0.5f, true, FG::Vector2D::Down * 2000.f, camera, { (float)SCREENWIDTH, (float)SCREENHEIGHT }));
	player->sprite = resourceManager->GetResource<FG::Sprite>("dodonpachi.png");
	player->StartPosition({500, 600});
	//player->position.x = 500.f;
	//player->position.y = 600.f;
	player->AddCircleCollider(32.f);
	entityManager->AddEntity(player);
	/*player->projectilePrefab = 
		new Projectile(resourceManager->GetResource<FG::Sprite>("bullet.png"), 0.5f, true, FG::Vector2D::Down*2000.f, camera, { (float)SCREENWIDTH, (float)SCREENHEIGHT });*/

	Obstacle* obstacle = new Obstacle(camera);
	obstacle->sprite = resourceManager->GetResource<FG::Sprite>("hippie.png");
	obstacle->position.x = 500.f;
	obstacle->position.y = 100.f;
	obstacle->AddCircleCollider(32);
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
