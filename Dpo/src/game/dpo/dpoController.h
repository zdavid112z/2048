#pragma once

struct DPOGameState;

class DPOController
{
public:
	virtual ~DPOController() {}
	// called once after a successful step with the future state
	// mainly used to have the ai thread work even while the animations are running
	virtual void PostStepPrepare(const DPOGameState& state) {}
	// called repeteadly during spawn animation
	// mainly used for human input
	virtual void Prepare() {}
	// is the step function ready to return for this state?
	virtual bool Ready(const DPOGameState&) = 0;
	// returns the direction for the current state
	// 0 = up, 1 = right, 2 = down, 3 = left
	virtual int Step(const DPOGameState&) = 0;
	// returns the name of the algorithm
	virtual string GetName() const = 0;
};