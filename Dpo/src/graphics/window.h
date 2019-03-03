#pragma once

#include "pch.h"
#include <SDL.h>
#include "input.h"
#include "renderer.h"
#include "timer.h"

class Window
{
public:
	Window(int, int, const string&);
	virtual ~Window();

	void Update();
	void OnResize();

	float left;
	int w, h;
	string title;
	SDL_Window* sdlWindow;
	SDL_Renderer* sdlRenderer;

	Timer* timer;
	Input* input;
	Renderer* renderer;

	uint maxFPS = 30;
	std::chrono::duration<float> maxFPSDuration;
	std::chrono::time_point<std::chrono::high_resolution_clock> maxFPSTime, maxFPSAux;
};
