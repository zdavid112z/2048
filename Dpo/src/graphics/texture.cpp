#include "pch.h"
#include "texture.h"
#include "renderer.h"
#include "window.h"

Texture::Texture(Renderer* r)
{
	renderer = r;
	texture = 0;
}

Texture::Texture(Renderer* r, const string& path)
{
	renderer = r;
	texture = 0;
	LoadImage(path);
}

Texture::~Texture()
{
	FreeTexture();
}

bool Texture::LoadImage(const string& path)
{
	FreeTexture();
	SDL_Surface* s = IMG_Load(path.c_str());
	if (!s) return false;
	w = s->w;
	h = s->h;
	texture = SDL_CreateTextureFromSurface(renderer->window->sdlRenderer, s);
	SDL_FreeSurface(s);
	if (!texture)
		return false;
	return true;
}

void Texture::CreateFromSurface(SDL_Surface* s)
{
	FreeTexture();
	w = s->w;
	h = s->h;
	texture = SDL_CreateTextureFromSurface(renderer->window->sdlRenderer, s);
}

void Texture::CreateBlank(int w, int h, uint format, int access)
{
	texture = SDL_CreateTexture(renderer->window->sdlRenderer, format, access, w, h);
	this->w = w;
	this->h = h;
}

void Texture::Update(const SDL_Rect* area, byte* pixels, uint pitch)
{
	SDL_UpdateTexture(texture, area, pixels, pitch);
}

void Texture::SetColor(Vector4i v)
{
	SDL_SetTextureColorMod(texture, v.r, v.g, v.b);
}

void Texture::SetAlpha(byte a)
{
	SDL_SetTextureAlphaMod(texture, a);
}

void Texture::SetBlendMode(BlendMode bm)
{
	SDL_SetTextureBlendMode(texture, SDL_BlendMode(bm));
}

void Texture::FreeTexture()
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = 0;
	}
}