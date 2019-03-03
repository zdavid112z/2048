#include "pch.h"
#include "input.h"
#include "window.h"

Input::Input(Window* wind)
{
	window = wind;
	memset(keys, 0, sizeof(keys));
	memset(mb, 0, sizeof(mb));
}

Input::~Input()
{

}

bool Input::IsKeyPressed(int k)
{
	return (keys[k] & 2) != 0;
}

bool Input::IsMouseButtonPressed(int k)
{
	return (mb[k] & 2) != 0;
}

int Input::GetKeyState(int k)
{
	return keys[k];
}

int Input::GetMouseButtonState(int k)
{
	return mb[k];
}

void Input::Update()
{
	for (int i = 0; i < 512; i++)
		if (keys[i] & 1)
			keys[i] ^= 1;
	for(int i = 0; i < 8; i++)
		if (mb[i] & 1)
			mb[i] ^= 1;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode >= 512)
				break;
			keys[event.key.keysym.scancode] = KEY_JUST_PRESSED;
			break;
		case SDL_KEYUP:
			if (event.key.keysym.scancode >= 512)
				break;
			keys[event.key.keysym.scancode] = KEY_JUST_RELEASED;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button >= 8)
				break;
			mb[event.button.button] = KEY_JUST_PRESSED;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button >= 8)
				break;
			mb[event.button.button] = KEY_JUST_RELEASED;
			break;
		case SDL_MOUSEMOTION:
			mx = ((float)event.motion.x / window->w) * (2.f * window->left) - window->left;
			my = -(((float)event.motion.y / window->h) * 20.f - 10.f);
			break;
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				window->w = event.window.data1;
				window->h = event.window.data2;
				window->OnResize();
				break;
			default:
				break;
			}
			break;
		case SDL_QUIT:
			shouldClose = true;
			break;
		default:
			break;
		}
	}
}