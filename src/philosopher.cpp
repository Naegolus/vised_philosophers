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

void Philosopher::setHisForks(ForkToken *leftFork, ForkToken *rightFork)
{
	_leftFork = leftFork->data();
	_rightFork = rightFork->data();

	acquireForks.addInput(leftFork);
	acquireForks.addInput(rightFork);

	releaseForks.addOutput(leftFork);
	releaseForks.addOutput(rightFork);
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
		if(acquireForks.fired())
			_state = StateEating;
		break;
	case StateEating:

		startedEating(this);
		_fib.calc(2);

		_state = StateWaitForThinking;
		break;
	case StateWaitForThinking:
		if(releaseForks.fired())
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
