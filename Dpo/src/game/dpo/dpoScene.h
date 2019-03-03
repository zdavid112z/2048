#pragma once

#include "../scene.h"
#include "dpoGame.h"
#include "../uiUtils.h"

class DPOScene : public Scene
{
public:
	DPOScene(Game* g);
	~DPOScene();
	void Update() override;
	void Render() override;
private:
	void Reset();
	void RenderBoard();
	void RenderTile(Vector2i coord, int value, float size = 1);
	void RenderTile(Vector2f coord, int value, float size = 1);
	void RenderTile(Vector2f coord, float size = 1);
	void RenderMovements(float amount);
	void RenderSpawnsAndJoins(float amount);

	Vector2f ToScreenCoords(Vector2i coords);

	Vector2f boardblCorner = Vector2f(-6, -4.5);
	float tilePadding = 0.5f;
	float tileSize = 2.5f;
	float moveDurationStandard = 0.18f;
	float spawnDurationStandard = 0.12f;
	float moveDuration, spawnDuration;
	float animationsDuration;
	float speedMultiplier = 1;
	float timer = 0;
	bool autorestart = true;
	float autorestartDuration = 3.f;
	float autorestartTimer;
	bool lost = false;
	bool benchmark = false;
	Label losingLabel;
	Label numbers[20];
	Vector4i colors[20];
	DPOGame dpogame;
	DPOController* humanController;
	DPOController* expectimaxController;
	DPOController* controller;
	Button humanControllerButton;
	Button expectimaxControllerButton;
	Label scoreLabel;
	Slider speedSlider;

	FILE* benchmarkLogFile = NULL;
	int benchmarkMaxTiles[30];
	int benchmarkMinTile = 30, benchmarkMaxTile = 0;
	long long benchmarkAverageScore = 0;
	int benchmarkMinScore = 0x3f3f3f3f, benchmarkMaxScore = 0;
	int benchmarkCurrentGame = 1;
	int benchmarkNumMoves = 0;
};