#pragma once

namespace FG
{
	// Abstract class, we only have pure virtuals
	class Application
	{
	public:
		virtual ~Application() {}

		virtual bool Initialize() = 0; // Pure virtual
		virtual void Run() = 0;
		virtual void Shutdown() = 0;
	};

	Application* CreateApplication();
}
