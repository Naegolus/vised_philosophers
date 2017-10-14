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

#include "Object.h"
#include "Fork.h"
#include "Fibonacci.h"
#include "PetriNet.h"

class Philosopher : public Object
{
public:
	Philosopher();
	virtual ~Philosopher();

	void setId(uint32_t id);
	uint32_t id() const;

	void setHisForks(Fork *leftFork, Fork *rightFork);

	void doStuff();
	uint32_t remainingThinkingCycles() const;
	bool isFinished() const;

	/* signals */
	signal0<> finished; /* because of thread. bad: better map in application */
	/* begin optional */
	  signal1<Philosopher *> startedEating;
	  signal1<Philosopher *> startedThinking;
	  signal1<Philosopher *> isHungry;
	  signal1<Philosopher *> finishedThinking;
	/* end optional */

private:
	uint32_t mId;

	Fork *leftFork;
	Fork *rightFork;

	Transition acquireForks;
	Transition releaseForks;

	/* for deadlock test only */
	Transition acquireLeftFork;
	Transition acquireRightFork;

	typedef enum
	{
		StateStartup = 0,
		StateHungry,
		StateAcquireRightFork, /* for deadlock test only */
		StateEating,
		StateWaitForThinking,
		StateThinking,
		StateDone
	} PhilosopherState;

	const uint32_t NumThinkingCycles = 3;

	PhilosopherState state;
	Fibonacci fib;
	uint32_t remThinkCycs;
};

#endif /* SRC_PHILOSOPHER_H_ */