#pragma once

#include "pch.h"
#include "dpoController.h"
#include "math\math.h"
#include <random>

struct DPOGameState
{
	// -1 means empty, 0 means 2, 1 means 4, 2 means 8 etc.
	int tiles[4][4];
};

struct DPOTileMovement
{
	DPOTileMovement(int value, Vector2i oldPos, Vector2i newPos) :
		value(value),
		oldPosition(oldPos),
		newPosition(newPos) {}

	int value;
	Vector2i oldPosition;
	Vector2i newPosition;
	Vector2f oldPositionScreen;
	Vector2f newPositionScreen;
};

struct DPOTileSpawn
{
	DPOTileSpawn(int value, Vector2i spawnPosition) :
		value(value),
		spawnPosition(spawnPosition) {}

	int value;
	Vector2i spawnPosition;
	Vector2f spawnPositionScreen;
};

struct DPOTileJoin
{
	DPOTileJoin(int value, Vector2i joinPosition) :
		value(value),
		joinPosition(joinPosition) {}

	int value;
	Vector2i joinPosition;
	Vector2f joinPositionScreen;
};

class DPOGame
{
public:
	DPOGame();
	void Reset(DPOGameState& state, bool addTileChanges);
	int Update(DPOController* controller);
	// returns the score if there was a change, -1 if lost or 0 if no change was made
	int Update(DPOGameState& state, DPOController* controller, bool addTileChanges);
	const DPOGameState& GetState() const;
	// sum of all the tiles
	int GetScore() const;
	// returns -1 if there's no more space
	// otherwise returns the generated tile
	int GenerateTiles(DPOGameState& state, int direction, bool addTileChanges);
	void Move(DPOGameState& state, int direction, bool addTileChanges);
	bool StatesDifferent(const DPOGameState& a, const DPOGameState& b) const;
	bool CanMove(DPOGameState a);

	float chanceToSpawn2 = 0.9f;
	std::vector<DPOTileMovement> movements;
	std::vector<DPOTileSpawn> spawns;
	std::vector<DPOTileJoin> joins;

private:
	int score = 0;
	std::mt19937 randEngine;
	DPOGameState currentState;
};