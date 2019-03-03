#include "pch.h"
#include "window.h"

Window::Window(int w, int h, const string& t)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF);
	TTF_Init();

	this->w = w;
	this->h = h;
	title = t;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_RESIZABLE);
	if (sdlWindow == NULL)
		LOG_INFO("Cannot create window!\n%s\n", SDL_GetError());
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
	if (sdlRenderer == NULL)
	{
		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
		if(sdlRenderer == NULL)
			LOG_INFO("Cannot create renderer!\n%s\n", SDL_GetError());

	}
	SDL_Surface* icon = IMG_Load("res/icon.png");
	SDL_SetWindowIcon(sdlWindow, icon);
	SDL_FreeSurface(icon);
	timer = new Timer();
	input = new Input(this);
	renderer = new Renderer(this);
	OnResize();
	maxFPSTime = std::chrono::high_resolution_clock::now();
}

Window::~Window()
{
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);

	delete input;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Window::Update()
{
	float interval = 1.f / (float)maxFPS;
	maxFPSDuration.zero();
	do
	{
		maxFPSAux = std::chrono::high_resolution_clock::now();
		maxFPSDuration = maxFPSAux - maxFPSTime;
	} while (maxFPSDuration.count() < interval);
	maxFPSTime = maxFPSAux;
	SDL_RenderPresent(sdlRenderer);
	input->Update();
	timer->Update();
}

void Window::OnResize()
{
	left = 10.f * w / h;
}
