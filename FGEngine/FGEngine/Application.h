#pragma once

namespace FG
{
	class Application
	{
	public:
		virtual ~Application() {}

		virtual bool Initialize() = 0;
		virtual void Run() = 0;
		virtual void Shutdown() = 0;
	};

	Application* CreateApplication();
}
