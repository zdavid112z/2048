#pragma once

#include "pch.h"
#include "graphics\window.h"
#include "scene.h"
#include "utils\xml.h"

class Game
{
public:
	static Game* Instance;

	Game(int w, int h, const string& title);
	virtual ~Game();
	virtual void Start();
	virtual void Stop();
	virtual void Loop();
	virtual void Update();
	virtual void Render();

	bool running = false;
	Window* window;
	Scene* scene = nullptr;
	Scene* nextScene = nullptr;
	XMLFile* cfg;
};
