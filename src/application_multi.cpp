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

typedef lock_guard<mutex> Lock;

Application::Application() :
			_appRunning(true),
			_philosophers(0),
			_threads(0),
			_forkToken(0)
{
}

Application::~Application()
{
	delete[] _philosophers;
	delete[] _threads;
	delete[] _forkToken;
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
			cerr << "Error: At least two philosophers must be dining." << endl;
			return 1;
		}
		createObjects(numObjects);
	}else
		createObjects(5);

	connectObjects();

	/* start threads as soon as everything is connected */
	for(uint32_t i = 0; i < _numPhilosophers; ++i)
		_threads[i].start(MAIN_INTERVAL);

	while(!allPhilosophersFinished())
		this_thread::sleep_for(interval);

	if(raceConditionDetected())
		cout << endl << "  Result: Race condition detected!" << endl;
	else
		cout << endl << "  Result: No race condition detected." << endl;

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
	_threads = new ThreadLoop[count];
	_forkToken = new ForkToken[count];
}

#define PRODUCE_RACE_CONDITION 0
void Application::connectObjects()
{
	uint32_t n;
	Philosopher *phil = _philosophers;
	ThreadLoop *thread = _threads;
	ForkToken *token = _forkToken;
#if not(PRODUCE_RACE_CONDITION)
	ForkToken *tokenRight;
#endif

		/* bind forks to token
		 * Must be done before executing function setHisForks()
		 * because of tokenRight -> seg fault */
	for(uint32_t i = 0; i < _numPhilosophers; ++i)
	{
		token->bind(_tableNr44.fork(i));
		++token;
	}

	token = _forkToken;
	for(uint32_t i = 0; i < _numPhilosophers; ++i)
	{
		thread->ticked.connect(phil, &Philosopher::doStuff);
		phil->finished.connect(thread, &ThreadLoop::requestFinish);

		/* bind fork token to philosophers */
		if(0 != i)
		{
			n = i - 1;
#if not(PRODUCE_RACE_CONDITION)
			tokenRight = token - 1;
#endif
		}else{
			n = _numPhilosophers - 1;
#if not(PRODUCE_RACE_CONDITION)
			tokenRight = &token[n];
#endif
		}

		phil->setId(i);
#if not(PRODUCE_RACE_CONDITION)
		phil->setHisForks(token, tokenRight);
#else
		phil->setHisForks(_tableNr44.fork(i), _tableNr44.fork(n));
#endif

		/* optional begin */
			phil->startedEating.connect(this, &Application::onPhilosopherStartedEating);
			phil->startedThinking.connect(this, &Application::onPhilosopherStartedThinking);
			phil->isHungry.connect(this, &Application::onPhilosopherIsHungry);
			phil->finishedThinking.connect(this, &Application::onPhilosopherFinishedThinking);
		/* optional end */

		++phil;
		++thread;
		++token;
	}
}

bool Application::allPhilosophersFinished()
{
	ThreadLoop *thread = _threads;

	for(uint32_t i = 0; i < _numPhilosophers; ++i)
	{
		if(!thread->isFinished())
			return false;

		++thread;
	}

	return true;
}

bool Application::raceConditionDetected()
{
	for(uint32_t i = 0; i < _numPhilosophers; ++i)
	{
		if(_tableNr44.fork(i)->dirtyCount())
			return true;
	}

	return false;
}

/* optional begin */
void Application::onPhilosopherStartedEating(Philosopher *p)
{
	Lock lock(_mtxCout);

	cout << "  " << p->id() << " started eating" << endl;
}

void Application::onPhilosopherStartedThinking(Philosopher *p)
{
	Lock lock(_mtxCout);

	cout << "  " << p->id() << " started thinking. Remaining thoughts: " << p->remainingThinkingCycles() << endl;
}

void Application::onPhilosopherIsHungry(Philosopher *p)
{
	Lock lock(_mtxCout);

	cout << "  " << p->id() << " is hungry" << endl;
}

void Application::onPhilosopherFinishedThinking(Philosopher *p)
{
	Lock lock(_mtxCout);

	cout << "  " << p->id() << " finished thinking" << endl;
}
/* optional end */
