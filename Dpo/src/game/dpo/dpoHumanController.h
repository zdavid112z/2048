#pragma once

#include "dpoController.h"
#include "../game.h"
#include "../scene.h"

class DPOHumanController : public DPOController
{
public:
	DPOHumanController(Scene* scene) : scene(scene)
	{
		direction = -1;
	}

	// true if direction has changed
	bool GetInput()
	{
		Input* input = scene->game->window->input;
		if (input->GetKeyState(SDL_SCANCODE_UP) == KEY_JUST_PRESSED || input->GetKeyState(SDL_SCANCODE_W) == KEY_JUST_PRESSED)
			direction = 0;
		else if (input->GetKeyState(SDL_SCANCODE_RIGHT) == KEY_JUST_PRESSED || input->GetKeyState(SDL_SCANCODE_D) == KEY_JUST_PRESSED)
			direction = 1;
		else if (input->GetKeyState(SDL_SCANCODE_DOWN) == KEY_JUST_PRESSED || input->GetKeyState(SDL_SCANCODE_S) == KEY_JUST_PRESSED)
			direction = 2;
		else if (input->GetKeyState(SDL_SCANCODE_LEFT) == KEY_JUST_PRESSED || input->GetKeyState(SDL_SCANCODE_A) == KEY_JUST_PRESSED)
			direction = 3;
		if (direction == -1)
			return false;
		return true;
	}

	void Prepare() override
	{
		GetInput();
	}

	bool Ready(const DPOGameState& state) override
	{
		return GetInput();
	}

	// 0 = up, 1 = right, 2 = down, 3 = left
	int Step(const DPOGameState& state) override
	{
		int old = direction;
		direction = -1;
		return old;
	}

	string GetName() const override
	{
		return "Human";
	}
private:
	int direction;
	Scene* scene;
};