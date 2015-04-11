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

#include "philosopher.h"

Philosopher::Philosopher() :
			_id(0),
			_leftFork(0),
			_rightFork(0),
			_state(StateStartup),
			_remainingThinkingCycles(NumThinkingCycles)
{
}

Philosopher::~Philosopher()
{
}

void Philosopher::setId(uint32_t id)
{
	_id = id;
}

uint32_t Philosopher::id() const
{
	return _id;
}

void Philosopher::setHisForks(Fork *leftFork, Fork *rightFork)
{
	/* bind forks to internal token -> access to resource is always granted */
	_leftForkToken.bind(leftFork);
	_rightForkToken.bind(rightFork);

	setHisForks(&_leftForkToken, &_rightForkToken);
}

#define PRODUCE_DEADLOCK 0
void Philosopher::setHisForks(ForkToken *leftFork, ForkToken *rightFork)
{
	_leftFork = leftFork->data();
	_rightFork = rightFork->data();

#if not(PRODUCE_DEADLOCK)
	_acquireForks.addInput(leftFork);
	_acquireForks.addInput(rightFork);
#else
	_acquireLeftFork.addInput(leftFork);
	_acquireRightFork.addInput(rightFork);
#endif

	_releaseForks.addOutput(leftFork);
	_releaseForks.addOutput(rightFork);
}

void Philosopher::doStuff()
{
	switch(_state)
	{
	case StateStartup:
		_state = StateHungry;
		isHungry(this);
		break;
	case StateHungry:
#if not(PRODUCE_DEADLOCK)
		if(_acquireForks.fired())
			_state = StateEating;
#else
		if(_acquireLeftFork.fired())
			_state = StateAcquireRightFork;
#endif
		break;
	case StateAcquireRightFork:

		_fib.calc(5); /* for deadlock test only */

		if(_acquireRightFork.fired())
			_state = StateEating;
		break;
	case StateEating:

		startedEating(this);
		_leftFork->makeDirty();
		_rightFork->makeDirty();
		_leftFork->makeClean();
		_rightFork->makeClean();

		_state = StateWaitForThinking;
		break;
	case StateWaitForThinking:
		if(_releaseForks.fired())
			_state = StateThinking;
		break;
	case StateThinking:

		startedThinking(this);
		_fib.calc(3);

		if(--_remainingThinkingCycles)
		{
			_state = StateHungry;
			isHungry(this);
		}else{
			_state = StateDone;
			finished();
			finishedThinking(this);
		}
		break;
	case StateDone:
		break;
	}
}

bool Philosopher::isFinished()
{
	return StateDone == _state;
}
