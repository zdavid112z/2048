#include "pch.h"
#include "dpoTreeAI.h"

#include "game/game.h"

#include "graphics/timer.h"

DPOTreeAI::DPOTreeAI(const DPOGame& game) : game(game)
{
	std::random_device rd;
	randEngine = std::mt19937(rd());
	running = true;
	aiThread = std::thread(&DPOTreeAI::ThreadMain, this);

	Game::Instance->cfg->SetVariable<int>("dpo/maxDepth", maxDepth);
	Game::Instance->cfg->SetVariable<int>("dpo/maxCheck", maxCheck);

	Game::Instance->cfg->SetVariable<float>("dpo/penaltyAmount", penaltyAmount);
	Game::Instance->cfg->SetVariable<float>("dpo/emptyTileBonus", emptyTileBonus);
	Game::Instance->cfg->SetVariable<float>("dpo/positionalBonus", positionalBonus);
	Game::Instance->cfg->SetVariable<float>("dpo/ascendingTilesBonus", ascendingTilesBonus);
	Game::Instance->cfg->SetVariable<float>("dpo/losingPenalty", losingPenalty);
}

DPOTreeAI::~DPOTreeAI()
{
	running = false;
	aiThread.join();
}

bool DPOTreeAI::Ready(const DPOGameState& state)
{
	if (working)
		return false;
	if (resultedDirectionAvailable)
		return true;
	if (!workAvailable)
	{
		work = state;
		workAvailable = true;
	}
	return false;
}

void DPOTreeAI::PostStepPrepare(const DPOGameState& state)
{
	work = state;
	workAvailable = true;
}

float DPOTreeAI::CalcScore(const DPOGameState& state)
{
	bool lost = !game.CanMove(state);
	if (lost)
		return -losingPenalty;
	int asc = 0;
	for (int i = 0; i < 4; i++)
	{
		int last = 40;
		for (int j = 0; j < 4; j++)
		{
			if (state.tiles[i][j] == -1)
				continue;
			if (state.tiles[i][j] == last || state.tiles[i][j] == last - 1 || last == 40)
			{
				if (last != 40)
					asc += Pow<int>(3, last + 1) * (4 - i);
				last = state.tiles[i][j];
			}
			else break;
		}
	}
	for (int j = 0; j < 4; j++)
	{
		int last = 40;
		for (int i = 0; i < 4; i++)
		{
			if (state.tiles[i][j] == -1)
				continue;
			if (state.tiles[i][j] == last || state.tiles[i][j] == last - 1 || last == 40)
			{
				if (last != 40)
					asc += Pow<int>(3, last + 1) * (4 - j);
				last = state.tiles[i][j];
			}
			else break;
		}
	}
	int penalty = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			penalty += Pow(3, abs(state.tiles[i][j] - state.tiles[i][j + 1]));
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 3; i++)
			penalty += Pow(3, abs(state.tiles[i][j] - state.tiles[i + 1][j]));

	int emptyTiles = 0;
	int score = 0;
	int posValue = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (state.tiles[i][j] == -1)
				emptyTiles++;
			else
			{
				int tileVal = Pow<int>(3, state.tiles[i][j] + 1);
				score += tileVal;
				int diagValue = 7 - i - j;
				posValue += diagValue * tileVal;
			}
		}
	}

	return score + float(score) * emptyTiles * emptyTileBonus + float(asc) * ascendingTilesBonus + float(posValue) * positionalBonus - float(penalty) * penaltyAmount;
}

int DPOTreeAI::Step(const DPOGameState& state)
{
	resultedDirectionAvailable = false;
	return resultedDirection;
}

string DPOTreeAI::GetName() const
{
	return "Expectimax";
}

void DPOTreeAI::ThreadMain()
{
	while (running)
	{
		while (!workAvailable && running);
		if (running == false)
			break;

		working = true;
		workAvailable = false;
		Timer timer;

		root.state = work;
		root.score = CalcScore(root.state);
		float score;
		int direction;
		int emptyTiles = 0;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (root.state.tiles[i][j] == -1)
					emptyTiles++;
		if(emptyTiles <= maxCheck && maxDepth <= 3)
			Solve(&root, maxDepth + 1, score, direction);
		else Solve(&root, maxDepth, score, direction);
		resultedDirection = direction;
		resultedDirectionAvailable = true;

		timer.Update();
		float workDuration = timer.GetDeltaTime();
		//LOG_INFO("AI Took: %f s\n", workDuration);
		working = false;
	}
}

// returns the direction or -1 if losing or exceeded max depth
void DPOTreeAI::Solve(DPOTreeNode* node, int depth, float& maxScore, int& desiredDirection)
{
	maxScore = -10000;
	desiredDirection = -1;
	if (depth <= 0) return; // should not be true
	for (int i = 0; i < 4; i++)
	{
		// viable move?
		DPOTreeNode newNode;
		newNode.state = node->state;
		game.Move(newNode.state, i, false);
		//newNode.score = CalcScore(newNode.state);

		// if the move wasn't viable
		if (StatesEqual(newNode.state, node->state))
			continue;

		// score and weight
		vector< pair<float, float> > scores;
		// generate all outcomes
		int v[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
		std::shuffle(v, v + 16, randEngine);
		// skip tiles only if it becomes a problem
		int checked = maxDepth >= 4 ? 0 : -100;
		for (int j = 0; j < 16 && checked < maxCheck; j++)
		{
			int ti = v[j] / 4;
			int tj = v[j] % 4;
			// cannot generate here
			if (newNode.state.tiles[ti][tj] != -1)
				continue;
			checked++;
			for (int k = 0; k < 2; k++)
			{
				DPOTreeNode generatedNode;
				generatedNode.state = newNode.state;
				generatedNode.state.tiles[ti][tj] = k;
				float generatedScore = generatedNode.score = CalcScore(generatedNode.state);
				int generatedDir = -1;
				if (depth > 1) // the board is now ready, generate a new move
					Solve(&generatedNode, depth - 1, generatedScore, generatedDir);
				scores.push_back(std::make_pair(generatedScore, k == 0 ? game.chanceToSpawn2 : (1 - game.chanceToSpawn2)));
			}
		}
		
		float newScore = 0;
		for (uint j = 0; j < scores.size(); j++)
			newScore += scores[j].first * scores[j].second;
		newScore /= float(scores.size()) / 2;
		if (newScore >= maxScore)
		{
			maxScore = newScore;
			desiredDirection = i;
		}
	}
}

bool DPOTreeAI::StatesEqual(const DPOGameState& a, const DPOGameState& b)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (a.tiles[i][j] != b.tiles[i][j])
				return false;
	return true;
}