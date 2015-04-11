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
#include "application.h"

using namespace std;

typedef lock_guard<mutex> Lock;

Application::Application() :
			_appRunning(true),
			_tableNr44(NUM_PHILOSOPHERS),
			_philosophers(0),
			_threads(0)
{
	_philosophers = new Philosopher[NUM_PHILOSOPHERS];
	_threads = new ThreadLoop[NUM_PHILOSOPHERS];
	_forkToken = new ForkToken[NUM_PHILOSOPHERS];
}

Application::~Application()
{
	delete[] _philosophers;
	delete[] _threads;
	delete[] _forkToken;
}

void Application::execute()
{
	chrono::milliseconds interval(MAIN_INTERVAL);

	cout << "Executing multi threaded Application" << endl << endl;

	connectObjects();

	/* start threads as soon as everything is connected */
	for(uint32_t i = 0; i < NUM_PHILOSOPHERS; ++i)
		_threads[i].start(MAIN_INTERVAL);

	while(!allPhilosophersFinished())
		this_thread::sleep_for(interval);

	cout << endl << "Application finished" << endl;

}

void Application::connectObjects()
{
	Philosopher *phil = _philosophers;
	ThreadLoop *thread = _threads;
	ForkToken *token = _forkToken;
	ForkToken *tokenRight;

		/* bind forks to token
		 * Must be done before executing function setHisForks()
		 * because of tokenRight -> seg fault */
	for(uint32_t i = 0; i < NUM_PHILOSOPHERS; ++i)
	{
		token->bind(_tableNr44.fork(i));
		++token;
	}

	token = _forkToken;
	for(uint32_t i = 0; i < NUM_PHILOSOPHERS; ++i)
	{
		thread->ticked.connect(phil, &Philosopher::doStuff);
		phil->finishedThinking.connect(thread, &ThreadLoop::requestFinish);

		/* bind fork token to philosophers */
		if(0 != i)
			tokenRight = token - 1;
		else
			tokenRight = &token[NUM_PHILOSOPHERS - 1];

		phil->setHisForks(token, tokenRight);

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

	for(uint32_t i = 0; i < NUM_PHILOSOPHERS; ++i)
	{
		if(!thread->isFinished())
			return false;

		++thread;
	}

	return true;
}

/* optional begin */
void Application::onPhilosopherStartedEating()
{
	Lock lock(_mtxCout);

	cout << "<x> started eating" << endl;
}

void Application::onPhilosopherStartedThinking()
{
	Lock lock(_mtxCout);

	cout << "<x> started thinking. Remaining thoughts: " << endl;
}

void Application::onPhilosopherIsHungry()
{
	Lock lock(_mtxCout);

	cout << "<x> is hungry" << endl;
}

void Application::onPhilosopherFinishedThinking()
{
	Lock lock(_mtxCout);

	cout << "<x> finished thinking" << endl;
}
/* optional end */
