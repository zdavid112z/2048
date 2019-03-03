#pragma once

#include "pch.h"

class Renderer;
class Game;

class Scene
{
public:
	Scene(Game* g);
	virtual ~Scene() {}
	
	virtual void Render() {}
	virtual void Update() {}

	Game* game;
	Renderer* renderer;
};