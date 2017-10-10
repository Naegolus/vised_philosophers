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
			state(StateStartup),
			remThinkCycs(NumThinkingCycles)
{
}

Philosopher::~Philosopher()
{
}

void Philosopher::setId(uint32_t id)
{
	mId = id;
}

uint32_t Philosopher::id() const
{
	return mId;
}

void Philosopher::setHisForks(Fork *leftFork, Fork *rightFork)
{
	/* bind forks to internal token -> access to resource is always granted */
	leftForkToken.bind(leftFork);
	rightForkToken.bind(rightFork);

	setHisForks(&leftForkToken, &rightForkToken);
}

void Philosopher::doStuff()
{
	switch(state)
	{
	case StateStartup:
		state = StateHungry;
		isHungry(this);
		break;
	case StateHungry:
		if(forks.fired())
			state = StateEating;

		break;
	case StateAcquireRightFork:

		fib.calc(5); /* for deadlock test only */

		if(_acquireRightFork.fired())
			state = StateEating;
		break;
	case StateEating:

		startedEating(this);

		/* read something from data container */
		leftFork->makeDirty();
		rightFork->makeDirty();

		/* calculate something */
		fib.calc(40);

		/* write something to data container */
		leftFork->makeClean();
		rightFork->makeClean();

		state = StateWaitForThinking;
		break;
	case StateWaitForThinking:
		if(_releaseForks.fired())
			state = StateThinking;
		break;
	case StateThinking:

		startedThinking(this);

		if(--remThinkCycs)
		{
			state = StateHungry;
			isHungry(this);
		}else{
			state = StateDone;
			finished();
			finishedThinking(this);
		}
		break;
	case StateDone:
		break;
	}
}

uint32_t Philosopher::remainingThinkingCycles() const
{
	return remThinkCycs;
}

bool Philosopher::isFinished() const
{
	return StateDone == state;
}
