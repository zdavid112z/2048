#include "pch.h"
#include "dpoGame.h"

DPOGame::DPOGame()
{
	std::random_device rd;
	randEngine = std::mt19937(rd());
	Reset(currentState, false);
}

void DPOGame::Reset(DPOGameState& state, bool addTileChanges)
{
	score = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			state.tiles[i][j] = -1;

	std::uniform_int_distribution<int> uid(0, 15);
	int v[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	std::shuffle(v, v + 16, randEngine);
	std::uniform_int_distribution<int> uid2(2, 4);
	int numTiles = uid2(randEngine);
	std::uniform_real_distribution<float> urd(0, 1);

	for (int i = 0; i < numTiles; i++)
	{
		int ti = v[i] % 4;
		int tj = v[i] / 4;
		int newTile = urd(randEngine) > chanceToSpawn2 ? 1 : 0;
		state.tiles[ti][tj] = newTile;
		score += 1 << (newTile + 1);
	}
}

int DPOGame::Update(DPOController* controller)
{
	return Update(currentState, controller, true);
}

// returns the score if there was a change, -1 if lost or 0 if no change was made
int DPOGame::Update(DPOGameState& state, DPOController* controller, bool addTileChanges)
{
	if (controller->Ready(state))
	{
		movements.clear();
		joins.clear();
		spawns.clear();

		int direction = controller->Step(state);
		DPOGameState prevState = state;
		Move(state, direction, addTileChanges);
		bool statesDifferent = StatesDifferent(state, prevState);
		if (statesDifferent)
		{
			int newTile = GenerateTiles(state, direction, addTileChanges);
			if (newTile == -1)
				return -1;
			score += (1 << (newTile + 1));
			prevState = state;
			int i;
			for (i = 0; i < 4; i++)
			{
				Move(prevState, i, false);
				if (StatesDifferent(state, prevState))
					break;
			}
			if (i == 4)
				return -1;
		}
		else
		{
			movements.clear();
			joins.clear();
			return 0;
		}
		return score;
	}
	return 0;
}

bool DPOGame::CanMove(DPOGameState state)
{
	for (int i = 0; i < 4; i++)
	{
		DPOGameState prevState = state;
		Move(state, i, false);
		bool statesDifferent = StatesDifferent(state, prevState);
		if (statesDifferent)
			return true;
	}
	return false;
}

const DPOGameState& DPOGame::GetState() const { return currentState; }
int DPOGame::GetScore() const { return score; }

// returns -1 if there's no more space
// otherwise returns the generated tile
int DPOGame::GenerateTiles(DPOGameState& state, int direction, bool addTileChanges)
{
	std::uniform_real_distribution<float> urd(0, 1);
	int newTile = urd(randEngine) > chanceToSpawn2 ? 1 : 0;
	int v[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	std::shuffle(v, v + 16, randEngine);
	for (int i = 0; i < 16; i++)
	{
		if (state.tiles[v[i] % 4][v[i] / 4] == -1)
		{
			state.tiles[v[i] % 4][v[i] / 4] = newTile;
			if (addTileChanges)
				spawns.emplace_back(newTile, Vector2i(v[i] % 4, v[i] / 4));
			return newTile;
		}
	}
	return -1;
}

void DPOGame::Move(DPOGameState& state, int direction, bool addTileChanges)
{
	switch (direction)
	{
	case 0:
		for (int i = 0; i < 4; i++)
		{
			int last, canJoin = 1;
			for (last = 0; last < 4 && state.tiles[last][i] == -1; last++);
			if (last < 4)
			{
				std::swap(state.tiles[last][i], state.tiles[0][i]);
				if (addTileChanges)
					movements.emplace_back(state.tiles[0][i], Vector2i(last, i), Vector2i(0, i));
			}
			for (int j = last + 1, last = 0; j < 4; j++)
			{
				int value = state.tiles[j][i];
				if (value == -1) continue;
				state.tiles[j][i] = -1;
				if (value == state.tiles[last][i] && canJoin)
				{
					state.tiles[last][i]++;
					canJoin = 0;
					if (addTileChanges)
						joins.emplace_back(value + 1, Vector2i(last, i));
				}
				else
				{
					state.tiles[++last][i] = value;
					canJoin = 1;
				}
				if (addTileChanges)
					movements.emplace_back(value, Vector2i(j, i), Vector2i(last, i));
			}
		}
		break;
	case 2:
		for (int i = 0; i < 4; i++)
		{
			int last, canJoin = 1;
			for (last = 3; last >= 0 && state.tiles[last][i] == -1; last--);
			if (last >= 0)
			{
				std::swap(state.tiles[last][i], state.tiles[3][i]);
				if (addTileChanges)
					movements.emplace_back(state.tiles[3][i], Vector2i(last, i), Vector2i(3, i));
			}
			for (int j = last - 1, last = 3; j >= 0; j--)
			{
				int value = state.tiles[j][i];
				if (value == -1) continue;
				state.tiles[j][i] = -1;
				if (value == state.tiles[last][i] && canJoin)
				{
					state.tiles[last][i]++;
					canJoin = 0;
					if (addTileChanges)
						joins.emplace_back(value + 1, Vector2i(last, i));
				}
				else
				{
					state.tiles[--last][i] = value;
					canJoin = 1;
				}
				if (addTileChanges)
					movements.emplace_back(value, Vector2i(j, i), Vector2i(last, i));
			}
		}
		break;
	case 3:
		for (int i = 0; i < 4; i++)
		{
			int last, canJoin = 1;
			for (last = 0; last < 4 && state.tiles[i][last] == -1; last++);
			if (last < 4)
			{
				std::swap(state.tiles[i][last], state.tiles[i][0]);
				if (addTileChanges)
					movements.emplace_back(state.tiles[i][0], Vector2i(i, last), Vector2i(i, 0));
			}
			for (int j = last + 1, last = 0; j < 4; j++)
			{
				int value = state.tiles[i][j];
				if (value == -1) continue;
				state.tiles[i][j] = -1;
				if (value == state.tiles[i][last] && canJoin)
				{
					state.tiles[i][last]++;
					canJoin = 0;
					if (addTileChanges)
						joins.emplace_back(value + 1, Vector2i(i, last));
				}
				else
				{
					state.tiles[i][++last] = value;
					canJoin = 1;
				}
				if (addTileChanges)
					movements.emplace_back(value, Vector2i(i, j), Vector2i(i, last));
			}
		}
		break;
	case 1:
		for (int i = 0; i < 4; i++)
		{
			int last, canJoin = 1;
			for (last = 3; last >= 0 && state.tiles[i][last] == -1; last--);
			if (last >= 0)
			{
				std::swap(state.tiles[i][last], state.tiles[i][3]);
				if (addTileChanges)
					movements.emplace_back(state.tiles[i][3], Vector2i(i, last), Vector2i(i, 3));
			}
			for (int j = last - 1, last = 3; j >= 0; j--)
			{
				int value = state.tiles[i][j];
				if (value == -1) continue;
				state.tiles[i][j] = -1;
				if (value == state.tiles[i][last] && canJoin)
				{
					state.tiles[i][last]++;
					canJoin = 0;
					if (addTileChanges)
						joins.emplace_back(value + 1, Vector2i(i, last));
				}
				else
				{
					state.tiles[i][--last] = value;
					canJoin = 1;
				}
				if (addTileChanges)
					movements.emplace_back(value, Vector2i(i, j), Vector2i(i, last));
			}
		}
		break;
	}
}

bool DPOGame::StatesDifferent(const DPOGameState& a, const DPOGameState& b) const
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (a.tiles[i][j] != b.tiles[i][j])
				return true;
	return false;
}