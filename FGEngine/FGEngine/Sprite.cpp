#include "Sprite.h"

FG::Sprite::Sprite(Entity* parent, SDL_Texture* tex, int srcW, int srcH, int w, int h)
{
	this->parent = parent;
	this->tex = tex;
	src.x = src.y = 0;
	src.w = srcW;
	src.h = srcH;
	dst.w = w;
	dst.h = h;
}
