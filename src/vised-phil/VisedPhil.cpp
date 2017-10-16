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

#include <iostream>
#include "VisedPhil.h"
#include "config.h"

using namespace std;

VisedPhil::VisedPhil() :
	appRunning(true),
	numPhilosophers(0),
	forks(0),
	philosophers(0),
	threads(0)
{
}

VisedPhil::~VisedPhil()
{
	delete[] forks;
	delete[] philosophers;
	delete[] threads;
}

int VisedPhil::exec(int argc, char *argv[])
{
	chrono::milliseconds interval(CHECK_INTERVAL);

	cout << endl << "Executing '" << PACKAGE_STRING << "'" << endl << endl;

	if (1 < argc)
		numPhilosophers = atoi(argv[1]);
	else
		numPhilosophers = 5;

	appInit();

	while (appRunning) {
		this_thread::sleep_for(interval);
	}

	cout << endl << "Finished" << endl << endl;

	return 0;
}

void VisedPhil::appInit()
{
	uint32_t n;
	Philosopher *phil = philosophers = new Philosopher[numPhilosophers];
	ThreadLoop *thread = threads = new ThreadLoop[numPhilosophers];
	forks = new Fork[numPhilosophers];

	for(uint32_t i = 0; i < numPhilosophers; ++i) {
		thread->ticked.connect(phil, &Philosopher::cyclic);

		n = i ? i - 1 : numPhilosophers - 1;
		phil->setId(i);
		phil->bindForks(&forks[i], &forks[n]);
		phil->changed.connect(this, &VisedPhil::printStatus);

		++phil;
		++thread;
	}

	thread = threads;
	for(uint32_t i = 0; i < numPhilosophers; ++i) {
		thread->start();
		++thread;
	}
}

/* Only print status if something has changed. Format:
	Philosopher - Eating - Progress
	         10        x   |==== | 4/5
*/
void VisedPhil::printStatus()
{
	/* This function is executed by external threads.
		Therefore it's a critical section!
	*/
	Lock lock(mtxInternal);

	Philosopher *phil = philosophers;

#if 0
	if (system("CLS"))
		system("clear");
#endif

	for(uint32_t i = 0; i < numPhilosophers; ++i) {
		cout << phil->id();
		cout << " ";
		if (phil->isEating())
			cout << "x";
		else
			cout << " ";
		cout << " ";
		cout << phil->remainingCycles();
		cout << endl;

		++phil;
	}
}

