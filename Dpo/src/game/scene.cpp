#include "pch.h"
#include "scene.h"
#include "game.h"

Scene::Scene(Game* g) :
	game(g),
	renderer(g->window->renderer) {}