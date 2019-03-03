#pragma once

#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>

#include "math\math.h"
#include "texture.h"

class Window;

class Renderer
{
public:
	Renderer(Window*);
	~Renderer();

	inline void ConvertDimensions(Vector2f&);
	inline void ConvertCoords(Vector2f&);
	inline void ConvertDimY(float&);
	void SetColor(Vector4i color);
	void ClearScreen();
	void DrawLine(Vector2f a, Vector2f b);
	void DrawRect(Vector2f center, Vector2f size);
	void FillRect(Vector2f center, Vector2f size);
	void DrawRectBounds(Vector2f bl, Vector2f tr);
	void FillRectBounds(Vector2f bl, Vector2f tr);
	void FillPolygon(Vector2f* points, int numPoints);
	void FillCircle(Vector2f center, float radius);
	void DrawCircle(Vector2f center, float radius);
	void FillRoundRect(Vector2f center, Vector2f size, float radius);
	void FillRoundRectBounds(Vector2f bl, Vector2f tr, float radius);
	void DrawImage(Texture* texture, Vector2f destPos, Vector2f destWH, float angle, Vector2f srcPos = Vector2f(0, 0), Vector2f srcWH = Vector2f(1, 1), byte flip = 0, bool centered = true);

	Window* window;
private:
	Vector4i currentColor;
	vector<Sint16> aux1, aux2;
};