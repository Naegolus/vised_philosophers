/*
 -----------------------------------------------------------------------------
 -----------------------------------------------------------------------------
 --                                                                         --
 --  This file is part of the Visual Software Editor project                --
 --  https://github.com/Naegolus/vised                                      --
 --                                                                         --
 --  Author(s):                                                             --
 --      - Helmut, redrocket@gmx.at                                         --
 --                                                                         --
 -----------------------------------------------------------------------------
 --                                                                         --
 --  Copyright (C) 2015 Authors                                             --
 --                                                                         --
 --  This program is free software: you can redistribute it and/or modify   --
 --  it under the terms of the GNU General Public License as published by   --
 --  the Free Software Foundation, either version 3 of the License, or      --
 --  (at your option) any later version.                                    --
 --                                                                         --
 --  This program is distributed in the hope that it will be useful,        --
 --  but WITHOUT ANY WARRANTY; without even the implied warranty of         --
 --  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           --
 --  GNU General Public License for more details.                           --
 --                                                                         --
 --  You should have received a copy of the GNU General Public License      --
 --  along with this program. If not, see <http://www.gnu.org/licenses/>.   --
 --                                                                         --
 -----------------------------------------------------------------------------
 ----------------------------------------------------------------------------- */

#include "Philosopher.h"

Philosopher::Philosopher() :
			mId(0),
			leftFork(0),
			rightFork(0),
			state(StateHungry),
			remThinkCycs(NumThinkingCycles),
			changed(false)
{
}

Philosopher::~Philosopher()
{
}

void Philosopher::setId(uint32_t id)
{
	Lock lock(mtxInternal);
	mId = id;
}

uint32_t Philosopher::id() const
{
	Lock lock(mtxInternal);
	return mId;
}

void Philosopher::bindForks(Fork *leftFork, Fork *rightFork)
{
	Lock lock(mtxInternal);

	leftFork = left;
	rightFork = right;

	forks.bind(left);
	forks.bind(right);
}

void Philosopher::cyclic()
{
	Lock lock(mtxInternal);

	switch(state)
	{
	case StateHungry:
#ifndef PRODUCE_RACE_CONDITION
		if(forks.acquire())
#endif
			setState(StateEating);
		break;

	case StateEating:
		/* read something from data container */
		leftFork->makeDirty();
		rightFork->makeDirty();

		/* calculate something */
		sleep(5);

		/* write something to data container */
		leftFork->makeClean();
		rightFork->makeClean();

		forks.release();
		setState(StateThinking);
		break;

	case StateThinking:
		sleep(1);

		if(--remThinkCycs)
			setState(StateHungry);
		else
			setState(StateDone);
		break;

	case StateDone:
		break;
	}
}

bool Philosopher::isEating() const
{
	Lock lock(mtxInternal);
	return StateEating == state;
}

uint32_t Philosopher::remainingThinkingCycles() const
{
	Lock lock(mtxInternal);
	return remThinkCycs;
}

bool Philosopher::ackChanged()
{
	Lock lock(mtxInternal);

	bool tmpChanged = changed;
	changed = false;

	return tmpChanged;
}

void Philosopher::setState(PhilosopherState newState)
{
	Lock lock(mtxInternal);

	state = newState;
	changed = true;
}

