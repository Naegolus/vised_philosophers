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
#include "VisedPhil.h"

using namespace std;

typedef lock_guard<mutex> Lock;

VisedPhil::VisedPhil() :
			appRunning(true),
			philosophers(0),
			threads(0)
{
}

VisedPhil::~VisedPhil()
{
	delete[] philosophers;
	delete[] threads;
}

int VisedPhil::exec(int argc, char *argv[])
{
	chrono::milliseconds interval(CHECK_INTERVAL);

	cout << endl;
	cout << "Executing multi threaded VisedPhil" << endl << endl;

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

	/* start threads as soon as everything is connected */
	for(uint32_t i = 0; i < numPhilosophers; ++i)
		threads[i].start(MAIN_INTERVAL);

	while(!allPhilosophersFinished())
		this_thread::sleep_for(interval);

	if(raceConditionDetected())
		cout << endl << "  Result: Race condition detected!" << endl;
	else
		cout << endl << "  Result: No race condition detected." << endl;

	cout << endl << "VisedPhil finished" << endl;
	cout << endl;

	return 0;
}

void VisedPhil::createObjects(uint32_t count)
{
	numPhilosophers = count;

	cout << "  Creating " << count << " philosophers." << endl << endl;

	tableNr44.createForks(count);
	philosophers = new Philosopher[count];
	threads = new ThreadLoop[count];
}

void VisedPhil::connectObjects()
{
	uint32_t n;
	Philosopher *phil = philosophers;
	ThreadLoop *thread = threads;

	for(uint32_t i = 0; i < numPhilosophers; ++i)
	{
		thread->ticked.connect(phil, &Philosopher::doStuff);
		phil->finished.connect(thread, &ThreadLoop::requestFinish);

		/* bind fork token to philosophers */
		if(i)
			n = i - 1;
		else
			n = numPhilosophers - 1;

		phil->setId(i);
		phil->setHisForks(tableNr44.fork(i), tableNr44.fork(n));

		/* optional begin */
			phil->startedEating.connect(this, &VisedPhil::onPhilosopherStartedEating);
			phil->startedThinking.connect(this, &VisedPhil::onPhilosopherStartedThinking);
			phil->isHungry.connect(this, &VisedPhil::onPhilosopherIsHungry);
			phil->finishedThinking.connect(this, &VisedPhil::onPhilosopherFinishedThinking);
		/* optional end */

		++phil;
		++thread;
	}
}

bool VisedPhil::allPhilosophersFinished()
{
	ThreadLoop *thread = threads;

	for(uint32_t i = 0; i < numPhilosophers; ++i)
	{
		if(!thread->isFinished())
			return false;

		++thread;
	}

	return true;
}

bool VisedPhil::raceConditionDetected()
{
	for(uint32_t i = 0; i < numPhilosophers; ++i)
	{
		if(tableNr44.fork(i)->dirtyCount())
			return true;
	}

	return false;
}

/* optional begin */
void VisedPhil::onPhilosopherStartedEating(Philosopher *p)
{
	Lock lock(mtxCout);

	cout << "  " << p->id() << " started eating" << endl;
}

void VisedPhil::onPhilosopherStartedThinking(Philosopher *p)
{
	Lock lock(mtxCout);

	cout << "  " << p->id() << " started thinking. Remaining thoughts: " << p->remainingThinkingCycles() << endl;
}

void VisedPhil::onPhilosopherIsHungry(Philosopher *p)
{
	Lock lock(mtxCout);

	cout << "  " << p->id() << " is hungry" << endl;
}

void VisedPhil::onPhilosopherFinishedThinking(Philosopher *p)
{
	Lock lock(mtxCout);

	cout << "  " << p->id() << " finished thinking" << endl;
}
/* optional end */
