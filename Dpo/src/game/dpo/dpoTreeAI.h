#pragma once

#include "dpoController.h"
#include "dpoGame.h"
#include <thread>
#include <atomic>

struct DPOTreeNode
{
	float score;
	DPOGameState state;
};

class DPOTreeAI : public DPOController
{
public:
	DPOTreeAI(const DPOGame& game);
	~DPOTreeAI();

	bool Ready(const DPOGameState& state) override;
	void PostStepPrepare(const DPOGameState& state) override;
	float CalcScore(const DPOGameState& state);
	int Step(const DPOGameState& state);
	string GetName() const override;
private:
	void ThreadMain();
	// returns the direction or -1 if losing or exceeded max depth
	void Solve(DPOTreeNode* node, int depth, float& maxScore, int& desiredDirection);
	bool StatesEqual(const DPOGameState& a, const DPOGameState& b);

private:
	float penaltyAmount = 1.5f;
	float emptyTileBonus = 0.075f;
	float positionalBonus = 0.3f;
	float ascendingTilesBonus = 0.5f;
	float losingPenalty = 100000;

	int maxCheck = 6;
	int maxDepth = 3;
	DPOTreeNode root;
	DPOGame game;
	std::mt19937 randEngine;
	std::thread aiThread;
	std::atomic_bool workAvailable = false, running = false, working = false, resultedDirectionAvailable = false;
	int resultedDirection;
	DPOGameState work;
};