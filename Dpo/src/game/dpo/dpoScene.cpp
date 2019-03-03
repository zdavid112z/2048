#include "pch.h"
#include "dpoScene.h"
#include "dpoHumanController.h"
#include "dpoTreeAI.h"
#include "xlsxwriter.h"
#include "math\math.h"
#include "../uiUtils.h"

DPOScene::DPOScene(Game* g) : Scene(g)
{
	// game->window->maxFPS = 60;
	game->window->maxFPS = 9999999;
	moveDuration = moveDurationStandard * 0.66f;
	spawnDuration = spawnDurationStandard * 0.66f;
	animationsDuration = moveDuration + spawnDuration;
	timer = animationsDuration + 1;

	game->cfg->SetVariable<bool>("dpo/autorestart", autorestart);
	game->cfg->SetVariable<float>("dpo/autorestartDuration", autorestartDuration);

	colors[0]  = Vector4i(238, 228, 218, 255);
	colors[1]  = Vector4i(237, 224, 200, 255);
	colors[2]  = Vector4i(241, 177, 121, 255);
	colors[3]  = Vector4i(245, 149, 99, 255);
	colors[4]  = Vector4i(246, 124, 94, 255);
	colors[5]  = Vector4i(247, 94, 60, 255);
	colors[6]  = Vector4i(237, 206, 114, 255);
	colors[7]  = Vector4i(236, 203, 97, 255);
	colors[8]  = Vector4i(236, 200, 80, 255);
	colors[9]  = Vector4i(236, 196, 64, 255);
	colors[10]  = Vector4i(236, 193, 46, 255);
	colors[11] = Vector4i(255, 61, 61, 255);
	colors[12] = Vector4i(255, 30, 30, 255);
	colors[13] = Vector4i(255, 38, 88, 255);
	colors[14] = Vector4i(130, 64, 150, 255);
	colors[15] = Vector4i(132, 97, 150, 255);
	colors[16] = Vector4i(40, 0, 142, 255);
	colors[17] = Vector4i(0, 132, 188, 255);
	colors[18] = Vector4i(38, 173, 175, 255);
	colors[19] = Vector4i(126, 175, 174, 255);
	for (int i = 0; i < 20; i++)
	{
		numbers[i].InitTexture(renderer);
		if(i < 2)
			numbers[i].InitTextTexture(std::to_string(1<<(i+1)).c_str(), Vector4i(110, 110, 110, 255));
		else numbers[i].InitTextTexture(std::to_string(1 << (i + 1)).c_str(), Vector4i(255, 255, 255, 255));
		numbers[i].SetDimY(tileSize * 0.4f);
	}
	losingLabel.InitTexture(renderer);
	losingLabel.InitTextTexture("You lost! Press R to restart");
	losingLabel.pos.x = 0;
	losingLabel.pos.y = -6.f;
	losingLabel.SetDimX(4 * (tilePadding + tileSize));

	humanControllerButton.InitAll(renderer);
	humanControllerButton.InitTextDef("Manual");
	humanControllerButton.pos.x = -12;
	humanControllerButton.pos.y = -1.75f;
	humanControllerButton.SetDimY(1.5f);

	scoreLabel.InitTexture(renderer);
	scoreLabel.InitTextTexture("Score: 0");
	scoreLabel.pos.x = -16;
	scoreLabel.pos.y = 9;
	scoreLabel.centered = false;
	scoreLabel.SetDimY(1.5f);

	expectimaxControllerButton.InitAll(renderer);
	expectimaxControllerButton.InitTextDef("Expectimax");
	expectimaxControllerButton.pos.x = -12;
	expectimaxControllerButton.pos.y = -3.75f;
	expectimaxControllerButton.SetDimY(1.5f);

	speedSlider.center.x = 0;
	speedSlider.center.y = -8.f;
	speedSlider.dim.x = 12;
	speedSlider.dim.y = 0.5f;
	speedSlider.sliderDim.x = 0.5f;
	speedSlider.sliderDim.y = 1.5f;
	speedSlider.val = 0.66f;

	game->cfg->SetVariable<bool>("dpo/benchmark", benchmark);

	humanController = new DPOHumanController(this);
	expectimaxController = new DPOTreeAI(dpogame);
	controller = expectimaxController;

	if (benchmark)
	{
		string filePrefix = "DPOTest";
		string fileSuffix = ".txt";
		string lastFileName = filePrefix + fileSuffix;
		FILE* file = fopen(lastFileName.c_str(), "r");
		int attempt = 1;
		while (file != NULL)
		{
			fclose(file);
			lastFileName = filePrefix + " (" + String::ToString(attempt) + ")" + fileSuffix;
			file = fopen(lastFileName.c_str(), "r");
			attempt++;
		}
		file = fopen(lastFileName.c_str(), "w");
		benchmarkLogFile = file;
		memset(benchmarkMaxTiles, 0, sizeof(benchmarkMaxTiles));
	}
}

DPOScene::~DPOScene()
{
	if (benchmark)
		fclose(benchmarkLogFile);
	delete humanController;
	delete expectimaxController;
}

void DPOScene::Reset()
{
	scoreLabel.InitTextTexture("Score: 0");
	scoreLabel.SetDimY(1.5f);
	timer = animationsDuration + 1;
	dpogame = DPOGame();
	lost = false;
}

void DPOScene::Update()
{
	if (humanControllerButton.Update(game->window))
		controller = humanController;
	if (expectimaxControllerButton.Update(game->window))
		controller = expectimaxController;
	speedSlider.Update(game->window->input);
	float newSpeedMultiplier = speedSlider.val;
	newSpeedMultiplier = Clamp(newSpeedMultiplier, 0.01f, 1.f);
	if (newSpeedMultiplier != speedMultiplier)
	{
		timer *= newSpeedMultiplier / speedMultiplier;
		moveDuration = moveDurationStandard * newSpeedMultiplier;
		spawnDuration = spawnDurationStandard * newSpeedMultiplier;
		animationsDuration = moveDuration + spawnDuration;
		speedMultiplier = newSpeedMultiplier;
	}
	if (game->window->input->GetKeyState(SDL_SCANCODE_R) == KEY_JUST_PRESSED || autorestartTimer <= 0 && lost && autorestart)
	{
		Reset();
		return;
	}
	autorestartTimer -= game->window->timer->GetDeltaTime();
	if (timer >= animationsDuration && !lost)
	{
		int result = dpogame.Update(controller);
		if (result > 0) // changes
		{
			scoreLabel.InitTextTexture(("Score: " + String::ToString(result)).c_str());
			scoreLabel.SetDimY(1.5f);
			if(benchmark)
				benchmarkNumMoves++;
			controller->PostStepPrepare(dpogame.GetState());
			timer = 0;
			for (uint i = 0; i < dpogame.movements.size(); i++)
			{
				dpogame.movements[i].oldPositionScreen = ToScreenCoords(dpogame.movements[i].oldPosition);
				dpogame.movements[i].newPositionScreen = ToScreenCoords(dpogame.movements[i].newPosition);
			}
			for (uint i = 0; i < dpogame.spawns.size(); i++)
				dpogame.spawns[i].spawnPositionScreen = ToScreenCoords(dpogame.spawns[i].spawnPosition);
			for (uint i = 0; i < dpogame.joins.size(); i++)
				dpogame.joins[i].joinPositionScreen = ToScreenCoords(dpogame.joins[i].joinPosition);
		}
		else if (result == -1)
		{
			if (benchmark)
			{
				int score = 0;
				int maxTile = 0;
				for (int i = 0; i < 4; i++)
					for (int j = 0; j < 4; j++)
					{
						score += Pow<int>(2, 1 + dpogame.GetState().tiles[i][j]);
						maxTile = Max(maxTile, dpogame.GetState().tiles[i][j]);
					}
				LOG_INFO("Game #%d: Score: %d, Max tile: %d, Moves: %d\n", benchmarkCurrentGame, score, Pow<int>(2, 1 + maxTile), benchmarkNumMoves);
				fprintf(benchmarkLogFile, "Game #%d: Score: %d, Max tile: %d, Moves: %d\n", benchmarkCurrentGame, score, Pow<int>(2, 1 + maxTile), benchmarkNumMoves);
				benchmarkMaxTiles[maxTile]++;
				benchmarkMaxTile = Max(benchmarkMaxTile, maxTile);
				benchmarkMinTile = Min(benchmarkMinTile, maxTile);
				benchmarkMaxScore = Max(benchmarkMaxScore, score);
				benchmarkMinScore = Min(benchmarkMinScore, score);
				benchmarkAverageScore += score;
				LOG_INFO("Average score: %f\n", double(benchmarkAverageScore) / benchmarkCurrentGame);
				fprintf(benchmarkLogFile, "Average score: %f\n", double(benchmarkAverageScore) / benchmarkCurrentGame);
				for (int i = benchmarkMinTile; i <= benchmarkMaxTile; i++)
				{
					LOG_INFO("%# 5d: %.3f%%\n", Pow<int>(2, i + 1), double(benchmarkMaxTiles[i]) / benchmarkCurrentGame * 100.);
					fprintf(benchmarkLogFile, "%# 5d: %.3f%%\n", Pow<int>(2, i + 1), double(benchmarkMaxTiles[i]) / benchmarkCurrentGame * 100.);
				}
				fflush(benchmarkLogFile);
				benchmarkNumMoves = 0;
				benchmarkCurrentGame++;
			}
			lost = true;
			if (autorestart)
				autorestartTimer = autorestartDuration;
		}
	}
}

void DPOScene::Render()
{
	RenderBoard();
	if (timer >= animationsDuration)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (dpogame.GetState().tiles[i][j] != -1)
					RenderTile(Vector2i(i, j), dpogame.GetState().tiles[i][j]);
	}
	else
	{
		if (timer < moveDuration)
		{
			float amount = 1.f - (moveDuration - timer) / moveDuration;
			RenderMovements(Clamp01(amount));
		}
		else if(timer < animationsDuration)
		{
			controller->Prepare();
			float amount = 1.f - (spawnDuration - timer + moveDuration) / spawnDuration;
			RenderMovements(1);
			RenderSpawnsAndJoins(Clamp01(amount));
		}
		timer += game->window->timer->GetDeltaTime();
	}
	if(lost)
		losingLabel.Render(renderer);
	humanControllerButton.Render(renderer);
	expectimaxControllerButton.Render(renderer);
	speedSlider.Render(renderer);
	scoreLabel.Render(renderer);
}

void DPOScene::RenderBoard()
{
	renderer->SetColor(Vector4i(187, 173, 160, 255));
	renderer->FillRoundRectBounds(boardblCorner, boardblCorner + Vector2f(1) * 4 * (tilePadding + tileSize), 0.15f);
	renderer->SetColor(Vector4i(214, 205, 196, 255));
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			RenderTile(ToScreenCoords({ i, j }));
}

void DPOScene::RenderTile(Vector2f coord, float size)
{
	// renderer->FillRect(coord, Vector2f(tileSize) * size);
	renderer->FillRoundRect(coord, Vector2f(tileSize) * size, 0.15f);
}

Vector2f DPOScene::ToScreenCoords(Vector2i coords)
{
	Vector2f res = boardblCorner;
	res.x += (float(coords.y) + 0.5f) * (tilePadding + tileSize);
	res.y += (float(3 - coords.x) + 0.5f) * (tilePadding + tileSize);
	return res;
}

void DPOScene::RenderTile(Vector2i coord, int value, float size)
{
	RenderTile(ToScreenCoords(coord), value, size);
}

void DPOScene::RenderTile(Vector2f coord, int value, float size)
{
	renderer->SetColor(colors[value]);
	RenderTile(coord, size);
	numbers[value].pos = coord;
	Vector2f oldSize = numbers[value].dim;
	numbers[value].dim *= size;
	numbers[value].Render(renderer);
	numbers[value].dim = oldSize;
}

void DPOScene::RenderMovements(float amount)
{
	// 0 -> 1
	float hermite = amount * amount * (3 - 2 * amount);
	for (uint i = 0; i < dpogame.movements.size(); i++)
	{
		const DPOTileMovement& m = dpogame.movements[i];
		Vector2f position = m.oldPositionScreen.Lerp(m.newPositionScreen, hermite);
		if (m.oldPosition == m.newPosition)
			position = m.oldPositionScreen;
		RenderTile(position, m.value);
	}
}

void DPOScene::RenderSpawnsAndJoins(float amount)
{
	// 0 -> 1
	float hermite = amount * amount * (3 - 2 * amount);
	// 1 -> 1.25 -> 1
	// float sinval = sinf(M_PI * amount) * 0.25f + 1.f;
	float val = amount - 0.5f;
	//float sinval = (1 - (2 * amount - 1) * (2 * amount - 1)) * 0.25f + 1.f;
	float sinval = -val * val + 1.25f;
	for (uint i = 0; i < dpogame.spawns.size(); i++)
	{
		const DPOTileSpawn& s = dpogame.spawns[i];
		RenderTile(s.spawnPositionScreen, s.value, hermite);
	}
	for (uint i = 0; i < dpogame.joins.size(); i++)
	{
		const DPOTileJoin& j = dpogame.joins[i];
		RenderTile(j.joinPositionScreen, j.value, sinval);
	}
}
