#pragma once

namespace FG
{
	class IResource
	{
	public:
		virtual ~IResource() {}
		virtual void Dispose() = 0;

	};
}