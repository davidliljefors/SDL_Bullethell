#pragma once

#include <unordered_map>
#include <string>
#include <cassert>

namespace FG
{
	class IResource;
	typedef std::unordered_map<std::string, IResource*> ResourceMap_t;

	class ResourceManager
	{
	public:
		void Shutdown();

		template <typename T>
		T* GetResource(const std::string& id) const
		{
			assert(HasResource(id));
			return dynamic_cast<T*>(resources[id]);
		}

		bool AddResource(const std::string& id, IResource* resource);
		bool HasResource(const std::string& id) const;
	protected:
		mutable ResourceMap_t resources;
	};
}

