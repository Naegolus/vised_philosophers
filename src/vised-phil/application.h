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

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <cstdint>
#include <mutex>
#include "object.h"
#include "data_token.h"
#include "table.h"
#include "philosopher.h"
#include "thread_loop.h"

class Application : public Object
{
public:
	Application();
	virtual ~Application();

	int execute(int argc, char *argv[]);
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

	bool _appRunning;
	std::mutex _mtxCout;

	uint32_t _numPhilosophers;

	Table _tableNr44;
	Philosopher *_philosophers;
	ThreadLoop *_threads;
	ForkToken *_forkToken;

	const uint32_t MAIN_INTERVAL = 1;
	const uint32_t CHECK_INTERVAL = 100;
	const uint32_t THREAD_SHUTDOWN_TIMEOUT_MS = 1000;
};

#endif /* APPLICATION_H_ */