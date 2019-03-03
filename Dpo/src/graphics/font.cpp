#include "pch.h"
#include "font.h"

Font::Font(const string& name, uint size)
{
	font = TTF_OpenFont(name.c_str(), size);
}

Font::~Font()
{
	TTF_CloseFont(font);
}

Texture* Font::RenderText(Texture* t, const string& text, Vector4i color)
{
	SDL_Color c;
	c.r = color.r;
	c.g = color.g;
	c.b = color.b;
	c.a = color.a;
	SDL_Surface* s = TTF_RenderText_Blended(font, text.c_str(), c);
	t->CreateFromSurface(s);
	SDL_FreeSurface(s);
	return t;
}
