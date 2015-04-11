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

#ifndef SRC_PHILOSOPHER_H_
#define SRC_PHILOSOPHER_H_

#include "object.h"
#include "transition.h"
#include "fork.h"
#include "fibonacci.h"

typedef DataToken<Fork> ForkToken;

class Philosopher : public Object
{
public:
	Philosopher();
	virtual ~Philosopher();

	/* using Philosopher singlethreaded -> ForkToken are internal and distributed */
	void setHisForks(Fork *leftFork, Fork *rightFork);

	/* using Philosopher multithreaded -> ForkToken are external and centralized */
	void setHisForks(ForkToken *leftFork, ForkToken *rightFork);

	void doStuff();
	bool isFinished();

	/* signals */
	signal0<> startedEating;
	signal0<> startedThinking;
	signal0<> isHungry;
	signal0<> finishedThinking;

private:
	Fork *_leftFork;
	Fork *_rightFork;

	ForkToken _leftForkToken;
	ForkToken _rightForkToken;

	Transition acquireForks;
	Transition releaseForks;

	typedef enum
	{
		StateStartup = 0,
		StateHungry,
		StateEating,
		StateWaitForThinking,
		StateThinking,
		StateDone
	} PhilosopherState;

	const uint32_t NumThinkingCycles = 3;

	PhilosopherState _state;
	Fibonacci _fib;
	uint32_t _remainingThinkingCycles;
};

#endif /* SRC_PHILOSOPHER_H_ */
