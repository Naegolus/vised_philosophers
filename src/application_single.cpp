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

#include <iostream>
#include <stdlib.h>
#include "application.h"

using namespace std;

Application::Application() :
			_appRunning(false),
			_philosophers(0),
			_threads(0),
			_forkToken(0)
{
}

Application::~Application()
{
	delete[] _philosophers;
}

int Application::execute(int argc, char *argv[])
{
	chrono::milliseconds interval(MAIN_INTERVAL);

	cout << endl;
	cout << "Executing multi threaded Application" << endl << endl;

	if(1 < argc)
	{
		uint32_t numObjects = atoi(argv[1]);

		if(2 > numObjects)
		{
			cerr << "Error: At least two philosophers must be dining." << endl << endl;
			return 1;
		}
		createObjects(numObjects);
	}else
		createObjects(5);

	connectObjects();

	while(!allPhilosophersFinished())
	{
		for(uint32_t i = 0; i < _numPhilosophers; ++i)
			_philosophers[i].doStuff();

		this_thread::sleep_for(interval);
	}

	cout << endl << "Application finished" << endl;
	cout << endl;

	return 0;
}

void Application::createObjects(uint32_t count)
{
	_numPhilosophers = count;

	cout << "  Creating " << count << " philosophers." << endl << endl;

	_tableNr44.createForks(count);
	_philosophers = new Philosopher[count];
}

void Application::connectObjects()
{
	uint32_t n;
	Philosopher *phil = _philosophers;

	for(uint32_t i = 0; i < _numPhilosophers; ++i)
	{
		/* bind fork token to philosophers */
		n = 0 != i ? i - 1 : _numPhilosophers - 1;
		phil->setId(i);
		phil->setHisForks(_tableNr44.fork(i), _tableNr44.fork(n));

		/* optional begin */
			phil->startedEating.connect(this, &Application::onPhilosopherStartedEating);
			phil->startedThinking.connect(this, &Application::onPhilosopherStartedThinking);
			phil->isHungry.connect(this, &Application::onPhilosopherIsHungry);
			phil->finishedThinking.connect(this, &Application::onPhilosopherFinishedThinking);
		/* optional end */

		++phil;
	}
}

bool Application::allPhilosophersFinished()
{
	Philosopher *phil = _philosophers;

	for(uint32_t i = 0; i < _numPhilosophers; ++i)
	{
		if(!phil->isFinished())
			return false;

		++phil;
	}

	return true;
}

/* optional begin */
void Application::onPhilosopherStartedEating(Philosopher *p)
{
	cout << "  " << p->id() << " started eating" << endl;
}

void Application::onPhilosopherStartedThinking(Philosopher *p)
{
	cout << "  " << p->id() << " started thinking. Remaining thoughts: " << p->remainingThinkingCycles() << endl;
}

void Application::onPhilosopherIsHungry(Philosopher *p)
{
	cout << "  " << p->id() << " is hungry" << endl;
}

void Application::onPhilosopherFinishedThinking(Philosopher *p)
{
	cout << "  " << p->id() << " finished thinking" << endl;
}
/* optional end */
