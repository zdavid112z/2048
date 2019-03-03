#include "pch.h"
#include "game.h"

Game* Game::Instance = nullptr;

Game::Game(int w, int h, const string& title)
{
	if (Instance != nullptr)
		LOG_INFO("Two games were created!\n");
	Instance = this;
	cfg = XMLFileLoader::LoadFile("config.xml");
	window = new Window(w, h, title);
}

Game::~Game()
{
	if (cfg != nullptr)
		delete cfg;
	delete window;
}

void Game::Start()
{
	running = true;
	while (running && !window->input->shouldClose)
		Loop();
}

void Game::Stop()
{
	running = false;
}

void Game::Loop()
{
	Update();
	Render();
	window->Update();
	if (nextScene != nullptr)
	{
		delete scene;
		scene = nextScene;
		nextScene = nullptr;
	}
}

void Game::Update() { if (scene) scene->Update(); }
void Game::Render() { if (scene) scene->Render(); }