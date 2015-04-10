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

Application::Application() :
			_appRunning(false),
			_tableNr44(NUM_PHILOSOPHERS),
			_philosophers(0),
			_threads(0)
{
}

Application::~Application()
{
	delete[] _philosophers;
	delete[] _threads;
}

void Application::execute()
{
	chrono::milliseconds interval(MAIN_INTERVAL);

	cout << "Executing multi threaded Application" << endl << endl;

	_philosophers = new Philosopher[NUM_PHILOSOPHERS];
	_threads = new ThreadLoop[NUM_PHILOSOPHERS];

	for(uint32_t i = 0; i < NUM_PHILOSOPHERS; ++i)
	{
		_threads[i].ticked.connect(&_philosophers[i], &Philosopher::doStuff);
		_philosophers[i].finished.connect(&_threads[i], &ThreadLoop::requestFinish);
		_philosophers[i].finished.connect(this, &Application::onPhilosopherFinished);

		_threads[i].start(MAIN_INTERVAL);
	}

	while(_appRunning)
	{
		this_thread::sleep_for(interval);

		if(allPhilosophersFinished())
			_appRunning = false;
	}

	for(uint32_t i = 0; i < NUM_PHILOSOPHERS; ++i)
		_threads[i].shutdown(THREAD_SHUTDOWN_TIMEOUT_MS);

	cout << endl << "Application finished" << endl;

}

bool Application::allPhilosophersFinished()
{
	for(uint32_t i = 0; i < NUM_PHILOSOPHERS; ++i)
	{
		if(!_threads[i].isFinished())
			return false;
	}

	return true;
}

void Application::onPhilosopherFinished()
{
	_mtx_cout.lock();
		cout << "<x> done" << endl;
	_mtx_cout.unlock();
}
