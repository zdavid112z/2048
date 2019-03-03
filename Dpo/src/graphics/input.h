#pragma once

#include "pch.h"
#include <SDL.h>

#define KEY_RELEASED 0
#define KEY_JUST_RELEASED 1
#define KEY_PRESSED 2
#define KEY_JUST_PRESSED 3

class Window;

class Input
{
public:
	Input(Window*);
	~Input();

	void Update();
	int GetKeyState(int k);
	int GetMouseButtonState(int k);
	bool IsKeyPressed(int k);
	bool IsMouseButtonPressed(int k);

	float mx = 0, my = 0;
	char mb[8];
	char keys[512];
	bool shouldClose = 0;
	SDL_Event event;
	Window* window;
};