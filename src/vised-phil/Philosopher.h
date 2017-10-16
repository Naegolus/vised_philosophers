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

#ifndef SRC_PHILOSOPHER_H_
#define SRC_PHILOSOPHER_H_

#include "Object.h"
#include "Transition.h"
#include "Fork.h"

class Philosopher : public Object
{
public:
	Philosopher();
	virtual ~Philosopher();

	void setId(uint32_t id);
	uint32_t id();

	void bindForks(Fork *left, Fork *right);

	void cyclic();
	bool isEating();
	uint32_t remainingCycles();

	bool ackChanged();

private:
	typedef enum
	{
		StateHungry = 0,
		StateEating,
		StateThinking,
		StateDone
	} PhilosopherState;

	void setState(PhilosopherState newState);

	std::mutex mtxInternal;
	uint32_t mId;
	uint32_t remThinkCycs;
	bool changed;
	Fork *leftFork;
	Fork *rightFork;
	Transition forks;
	PhilosopherState state;

	const uint32_t NumThinkingCycles = 3;
};

#endif /* SRC_PHILOSOPHER_H_ */
