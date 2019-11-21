#include "ResourceManager.h"
#include "IResource.h"

#include <cassert>

namespace FG
{
	void ResourceManager::Shutdown()
	{
		for (auto it = resources.begin(); it != resources.end(); it++)
		{
			it->second->Dispose();
			delete it->second;
		}
		resources.clear();
	}
	bool ResourceManager::AddResource(const std::string& id, IResource* resource)
	{
		assert(!id.empty());
		assert(resource);

		if (HasResource(id))
		{
			return false;
		}
		resources[id] = resource;
		return true;
	}
	bool ResourceManager::HasResource(const std::string& id) const
	{
		return resources.find(id) != resources.end();
		//auto it = resources.find(id);
		//if (it != resources.end())
		//{
		//	return true;
		//}
		//return false;
	}
}