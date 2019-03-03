#include "pch.h"
#include "graphics\window.h"
#include "math\math.h"
#include "graphics\renderer.h"
#include "graphics\font.h"
#include "game\game.h"
#include "game\dpo\dpoScene.h"
#include "game\resources.h"

class MyGame : public Game
{
public:
	MyGame() :
		Game(1280, 720, "2048")
	{
		Resources::Load();
		scene = new DPOScene(this);
	}

	virtual ~MyGame()
	{
		Resources::Unload();
		delete scene;
	}

	virtual void Update()
	{
		t += window->timer->GetDeltaTime();
		if (t >= 1)
		{
			t--;
			LOG_VERB("FPS: %d\n", fps);
			fps = 0;
		}
		fps++;
		Game::Update();
	}

	virtual void Render()
	{
		window->renderer->SetColor(Vector4i(255, 255, 255, 255));
		window->renderer->ClearScreen();
		Game::Render();
	}

	float t = 0;
	int fps = 0;
};

int main(int argc, char** args)
{
	MyGame game;
	game.Start();
	return 0;
}
