#pragma once
#include "Collider.h"

namespace FG
{
	class BoxCollider : public Collider
	{

	public:


		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
	};

}