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

#ifndef VISED_PHIL_H_
#define VISED_PHIL_H_

#include <cstdint>
#include <mutex>
#include "Object.h"
#include "Table.h"
#include "Philosopher.h"
#include "ThreadLoop.h"

class VisedPhil : public Object
{
public:
	VisedPhil();
	virtual ~VisedPhil();

	int exec(int argc, char *argv[]);
	void createObjects(uint32_t count);
	void connectObjects();

	/* slots */
	void onPhilosopherStartedEating(Philosopher *p);
	void onPhilosopherStartedThinking(Philosopher *p);
	void onPhilosopherIsHungry(Philosopher *p);
	void onPhilosopherFinishedThinking(Philosopher *p);

private:
	bool allPhilosophersFinished();
	bool raceConditionDetected();

	bool appRunning;
	std::mutex mtxCout;

	uint32_t numPhilosophers;

	Table tableNr44;
	Philosopher *philosophers;
	ThreadLoop *threads;

	const uint32_t MAIN_INTERVAL = 1;
	const uint32_t CHECK_INTERVAL = 100;
	const uint32_t THREAD_SHUTDOWN_TIMEOUT_MS = 1000;
};

#endif /* VISED_PHIL_H_ */
