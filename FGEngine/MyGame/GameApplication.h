#pragma once

#include <Application.h>
#include <Time.h>

namespace FG
{
	class Window;
	class InputManager;
	class Camera;
	class EntityManager;
	class ResourceManager;
}

class GameApplication : public FG::Application
{
public:
	virtual bool Initialize() override;
	virtual void Run() override;
	virtual void Shutdown() override;

	const int SCREENWIDTH = 1024;
	const int SCREENHEIGHT = 768;

private:
	const float targetFramerate = 60;
	const float frameDelay = 1000 / targetFramerate;
	float frametime = 0;

	FG::Window* window = nullptr;
	FG::InputManager* inputManager = nullptr;
	FG::Camera* camera = nullptr;
	FG::EntityManager* entityManager = nullptr;
	FG::ResourceManager* resourceManager = nullptr;

	FG::Time time;
};

FG::Application* FG::CreateApplication()
{
	return new GameApplication();
}
