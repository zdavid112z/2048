#include "pch.h"
#include "renderer.h"
#include "window.h"

Renderer::Renderer(Window* w)
{
	window = w;
}

Renderer::~Renderer()
{

}

void Renderer::ConvertCoords(Vector2f& v)
{
	v.x = ((v.x + window->left) / window->left) / 2.f * window->w;
	v.y = ((-v.y + 10.f) / 10.f) / 2.f * window->h;
}

void Renderer::ConvertDimensions(Vector2f& v)
{
	v.x = (v.x / window->left) / 2.f * window->w;
	v.y = (v.y / 10.f) / 2.f * window->h;
}

void Renderer::ConvertDimY(float& d)
{
	d = (d / 10.f) / 2.f * window->h;
}

void Renderer::SetColor(Vector4i c)
{
	SDL_SetRenderDrawColor(window->sdlRenderer, c.r, c.g, c.b, c.a);
	currentColor = c;
}

void Renderer::ClearScreen()
{
	SDL_RenderClear(window->sdlRenderer);
}

void Renderer::DrawLine(Vector2f a, Vector2f b)
{
	ConvertCoords(a);
	ConvertCoords(b);
	SDL_RenderDrawLine(window->sdlRenderer, (int)a.x, (int)a.y, (int)b.x, (int)b.y);
}

void Renderer::DrawRect(Vector2f a, Vector2f b)
{
	ConvertCoords(a);
	ConvertDimensions(b);
	SDL_Rect r = { int(a.x - b.x / 2), int(a.y - b.y / 2), (int)b.x, (int)b.y };
	SDL_RenderDrawRect(window->sdlRenderer, &r);
}

void Renderer::FillRect(Vector2f a, Vector2f b)
{
	ConvertCoords(a);
	ConvertDimensions(b);
	SDL_Rect r = { int(a.x - b.x / 2), int(a.y - b.y / 2), (int)b.x, (int)b.y };
	SDL_RenderFillRect(window->sdlRenderer, &r);
}

void Renderer::DrawRectBounds(Vector2f bl , Vector2f size)
{
	ConvertCoords(bl);
	ConvertDimensions(size);
	SDL_Rect r = { (int)bl.x, (int)bl.y, (int)size.x, (int)size.y};
	SDL_RenderDrawRect(window->sdlRenderer, &r);
}

void Renderer::FillRectBounds(Vector2f bl, Vector2f size)
{
	ConvertCoords(bl);
	ConvertDimensions(size);
	SDL_Rect r = { (int)bl.x, (int)bl.y, (int)size.x, (int)size.y };
	SDL_RenderFillRect(window->sdlRenderer, &r);
}

void Renderer::FillPolygon(Vector2f* v, int n)
{
	aux1.clear();
	aux2.clear();
	for (int i = 0; i < n; i++)
	{
		ConvertCoords(v[i]);
		aux1.push_back((Sint16)v[i].x);
		aux2.push_back((Sint16)v[i].y);
	}
	filledPolygonRGBA(window->sdlRenderer, &aux1[0], &aux2[0], n, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
}

void Renderer::FillCircle(Vector2f pos, float r)
{
	ConvertCoords(pos);
	ConvertDimY(r);
	filledCircleRGBA(window->sdlRenderer, (Sint16)pos.x, (Sint16)pos.y, (Sint16)r, (Uint8)currentColor.r, (Uint8)currentColor.g, (Uint8)currentColor.b, (Uint8)currentColor.a);
}

void Renderer::DrawCircle(Vector2f pos, float r)
{
	ConvertCoords(pos);
	ConvertDimY(r);
	circleRGBA(window->sdlRenderer, (Sint16)pos.x, (Sint16)pos.y, (Sint16)r, (Uint8)currentColor.r, (Uint8)currentColor.g, (Uint8)currentColor.b, (Uint8)currentColor.a);
}

void Renderer::FillRoundRect(Vector2f center, Vector2f size, float radius)
{
	FillRoundRectBounds(center - size / 2., center + size / 2., radius);
}

void Renderer::FillRoundRectBounds(Vector2f bl, Vector2f tr, float radius)
{
	ConvertCoords(bl);
	ConvertCoords(tr);
	ConvertDimY(radius);
	roundedBoxRGBA(window->sdlRenderer, (Sint16)bl.x, (Sint16)bl.y, (Sint16)tr.x, (Sint16)tr.y, (Sint16)radius, (Uint8)currentColor.r, (Uint8)currentColor.g, (Uint8)currentColor.b, (Uint8)currentColor.a);
}

void Renderer::DrawImage(Texture* t, Vector2f destPos, Vector2f destWH, float angle, Vector2f srcPos, Vector2f srcWH, byte flip, bool centered)
{
	SDL_Rect dest;
	if (destWH.x == 0)
	{
		float iar = (float)((srcWH.x - srcPos.x) * t->w) / ((srcWH.y - srcPos.y) * t->h);
		destWH.x = destWH.y * iar;
	}
	else if (destWH.y == 0)
	{
		float iar = (float)((srcWH.x - srcPos.x) * t->w) / ((srcWH.y - srcPos.y) * t->h);
		destWH.y = destWH.x / iar;
	}
	ConvertCoords(destPos);
	ConvertDimensions(destWH);
	if (centered)
	{
		dest.x = int(destPos.x - destWH.x / 2);
		dest.y = int(destPos.y - destWH.y / 2);
	}
	else
	{
		dest.x = int(destPos.x);
		dest.y = int(destPos.y);
	}
	dest.w = (int)destWH.x;
	dest.h = (int)destWH.y;
	srcPos.x *= t->w; srcPos.y *= t->h;
	srcWH.x *= t->w; srcWH.y *= t->h;
	SDL_Rect src = { (int)srcPos.x, (int)srcPos.y, (int)srcWH.x, (int)srcWH.y };
	SDL_RenderCopyEx(window->sdlRenderer, t->texture, &src, &dest, angle, NULL, SDL_RendererFlip(flip));
}
