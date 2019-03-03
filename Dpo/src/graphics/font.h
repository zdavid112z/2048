#pragma once

#include "pch.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "texture.h"

class Font
{
public:
	Font(const string& name, uint size);
	~Font();
	Texture* RenderText(Texture*, const string&, Vector4i);

	TTF_Font* font;
};
