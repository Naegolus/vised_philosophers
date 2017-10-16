/*
  This file is part of the DSP-Crowd project
  https://www.dsp-crowd.com

  Author(s):
      - Johannes Natter, office@dsp-crowd.com

  Copyright (C) 2017 Authors and www.dsp-crowd.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <thread>
#include "Philosopher.h"

#define PRODUCE_RACE_CONDITION 0

using namespace std;

Philosopher::Philosopher()
{
	mId = 0;
	leftFork = 0;
	rightFork = 0;
	state = StateHungry;
	remThinkCycs = NumThinkingCycles;
}

Philosopher::~Philosopher()
{
}

/* init */
void Philosopher::setId(uint32_t id)
{
	Lock lock(mtxInternal);
	mId = id;
}
uint32_t Philosopher::id()
{
	Lock lock(mtxInternal);
	return mId;
}
void Philosopher::setThinkingCycles(uint32_t remThinkingCycles)
{
	Lock lock(mtxInternal);
	if (remThinkingCycles)
		remThinkCycs = remThinkingCycles;
}
void Philosopher::bindForks(Fork *left, Fork *right)
{
	Lock lock(mtxInternal);

	leftFork = left;
	rightFork = right;

	forks.bind(left);
	forks.bind(right);
}

/* cyclic */
void Philosopher::cyclic()
{
	switch(state) {
	case StateHungry:
#if not(PRODUCE_RACE_CONDITION)
		if(forks.acquire())
#endif
			setState(StateEating);
		break;

	case StateEating:
		/* read something from data container */
		leftFork->makeDirty();
		rightFork->makeDirty();

		/* write something to data container */
		leftFork->makeClean();
		rightFork->makeClean();

		forks.release();

		setState(StateThinking);
		break;

	case StateThinking:
		this_thread::sleep_for(chrono::milliseconds(200));

		if(decrementTinkCycles())
			setState(StateHungry);
		else
			setState(StateDone);
		break;

	case StateDone:
		break;
	}
}

/* states */
bool Philosopher::isEating()
{
	Lock lock(mtxInternal);
	return StateEating == state;
}
uint32_t Philosopher::remainingCycles()
{
	Lock lock(mtxInternal);
	return remThinkCycs;
}

/* internal functions */
void Philosopher::setState(PhilosopherState newState)
{
	{
		Lock lock(mtxInternal);
		state = newState;
	}
	/* signal must be emitted outside of state mutex */
	changed();
}
uint32_t Philosopher::decrementTinkCycles()
{
	Lock lock(mtxInternal);
	--remThinkCycs;
	return remThinkCycs;
}

