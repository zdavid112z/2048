#pragma once

#include "graphics\window.h"
#include "graphics/renderer.h"
#include "graphics\font.h"
#include "game/resources.h"

struct Button
{
	Texture* current = nullptr;
	Texture* normal = nullptr, *hover = nullptr, *click = nullptr;

	void InitAll(Renderer* r)
	{
		normal = new Texture(r);
		hover = new Texture(r);
		click = new Texture(r);
	}

	void InitTextDef(const char* text)
	{
		Resources::Calibri->RenderText(normal, text, Vector4i(30, 30, 30, 255));
		Resources::Calibri->RenderText(hover, text, Vector4i(140, 140, 140, 255));
		Resources::Calibri->RenderText(click, text, Vector4i(200, 30, 30, 255));
	}

	void DeleteAll()
	{
		delete normal;
		delete hover;
		delete click;
	}

	const float clickTime = 0.1f;
	float timer = 0;

	bool Update(Window* w)
	{
		current = normal;
		bool ins = MouseInside(w->input->mx, w->input->my), rez = false;
		if (ins)
		{
			current = hover;
			if (w->input->mb[1] == KEY_JUST_PRESSED)
			{
				rez = true;
				current = click;
				timer = clickTime;
			}
		}
		if (timer > 0)
		{
			current = click;
			timer -= w->timer->GetDeltaTime();
		}
		return rez;
	}

	void Render(Renderer* r)
	{
		r->DrawImage(current, pos, dim, 0, Vector2f(0, 0), Vector2f(1, 1), flip);
	}

	Vector2f pos, dim;
	byte flip = 0;

	inline bool MouseInside(float mx, float my) {
		return ((mx >= pos.x - dim.x / 2.) && (mx <= pos.x + dim.x / 2.) && (my >= pos.y - dim.y / 2.) && (my <= pos.y + dim.y / 2.));
	}

	void SetDimX(float x)
	{
		dim.x = x;
		dim.y = x * ((float)normal->h / normal->w);
	}

	void SetDimY(float y)
	{
		dim.y = y;
		dim.x = y * ((float)normal->w / normal->h);
	}
};

class Label
{
public:
	Texture* normal = nullptr;

	void InitTexture(Renderer* r)
	{
		normal = new Texture(r);
	}

	void InitTextTexture(const char* text, Vector4i color = Vector4i(30, 30, 30, 255))
	{
		Resources::Calibri->RenderText(normal, text, color);
	}

	void SetFloat(float val)
	{
		//Resources::Calibri->RenderText(normal, , Vector4i(30, 30, 30, 255));
	}

	void DeleteAll()
	{
		delete normal;
	}

	void Render(Renderer* r)
	{
		r->DrawImage(normal, pos, dim, 0, Vector2f(0, 0), Vector2f(1, 1), flip, centered);
	}

	void SetDimX(float x)
	{
		dim.x = x;
		dim.y = x * ((float)normal->h / normal->w);
	}

	void SetDimY(float y)
	{
		dim.y = y;
		dim.x = y * ((float)normal->w / normal->h);
	}

	Vector2f pos, dim;
	byte flip = 0;
	bool centered = true;
};


class Slider
{
public:
	Vector4i barColor, normalColor, hoverColor, clickColor, currentColor;
	Vector2f center, dim, sliderDim;
	float val = 0;
	bool grabbed = false;

	Slider()
	{
		barColor = Vector4i(80, 80, 80, 255);
		normalColor = Vector4i(66, 191, 244, 255);
		hoverColor = Vector4i(19, 155, 214, 255);
		clickColor = Vector4i(17, 114, 155, 255);
	}

	bool Inside(float mx, float my)
	{
		return mx >= center.x - dim.x / 2 - sliderDim.x / 2 &&
			mx <= center.x + dim.x / 2 + sliderDim.x / 2 &&
			my >= center.y - sliderDim.y / 2 &&
			my <= center.y + sliderDim.y / 2;
	}

	void Update(Input* input)
	{
		bool ins = Inside(input->mx, input->my);
		if (ins)
			currentColor = hoverColor;
		else currentColor = normalColor;
		if (input->mb[1] == KEY_JUST_PRESSED)
		{
			if (ins)
				grabbed = true;
			else grabbed = false;
		}
		else if (input->mb[1] == KEY_JUST_RELEASED)
			grabbed = false;
		if (grabbed)
		{
			currentColor = clickColor;
			val = Clamp01((input->mx - (center.x - dim.x / 2.f)) / dim.x);
		}
	}

	void Render(Renderer* r)
	{
		r->SetColor(barColor);
		r->FillRect(center, dim);
		r->SetColor(currentColor);
		r->FillRect(Vector2f(center.x - dim.x / 2.f + val * dim.x, center.y), sliderDim);
	}
};

