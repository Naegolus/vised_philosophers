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
#include <iomanip>
#include "VisedPhil.h"
#include "config.h"

#define PRODUCE_DIRTY_OUTPUT 0
#define USE_MULTI_THREADING  1

using namespace std;

VisedPhil::VisedPhil()
{
	appRunning = true;
	numPhilosophers = 5;
	numThinkingCycles = 3;
	forks = 0;
	philosophers = 0;
	threads = 0;
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

	if (2 < argc)
		numThinkingCycles = atoi(argv[2]);

	appInit();

	while (appRunning) {
#if not(USE_MULTI_THREADING)
		Philosopher *phil = philosophers;
		for(uint32_t i = 0; i < numPhilosophers; ++i) {
			phil->cyclic();
			++phil;
		}
#endif
		this_thread::sleep_for(interval);
	}

	cout << endl << "  Finished" << endl << endl;

	return 0;
}

void VisedPhil::appInit()
{
	uint32_t n;
	Philosopher *phil = philosophers = new Philosopher[numPhilosophers];
	ThreadLoop *thread = threads = new ThreadLoop[numPhilosophers];
	forks = new Fork[numPhilosophers];

	for(uint32_t i = 0; i < numPhilosophers; ++i) {
		n = i ? i - 1 : numPhilosophers - 1;
		phil->setId(i);
		phil->setThinkingCycles(numThinkingCycles);
		phil->bindForks(&forks[i], &forks[n]);
		phil->changed.connect(this, &VisedPhil::printStatusAndCheckShutdown);

#if USE_MULTI_THREADING
		thread->ticked.connect(phil, &Philosopher::cyclic);
		thread->start();
#endif
		++phil;
		++thread;
	}
}

/* Only print status if something has changed. Format:
	Philosopher - Eating - Progress
	         10        x   |==== | 4/5
*/
void VisedPhil::printStatusAndCheckShutdown()
{
	/* This function is executed by external threads.
		Therefore it's a critical section!
	*/
#if not(PRODUCE_DIRTY_OUTPUT)
	Lock lock(mtxInternal);
#endif

	bool applicationShutdown = true;
	Philosopher *phil = philosophers;
	uint32_t remCyc;

	system("clear");

	cout << endl;
	cout << "  Philosopher - Eating - Thinking Progress" << endl;
	for(uint32_t i = 0; i < numPhilosophers; ++i) {
		cout << "  ";
		cout << setw(11) << phil->id();
		cout << "   ";

		if (phil->isEating())
			cout << setw(6) << "x";
		else
			cout << setw(6) << " ";

		remCyc = phil->remainingCycles();
		cout << "   |";
		for (uint32_t n = 0; n < numThinkingCycles; ++n)
		{
			if (n < numThinkingCycles - remCyc)
				cout << "=";
			else
				cout << " ";
		}
		cout << "| " << (numThinkingCycles - remCyc) << "/" << numThinkingCycles;

		cout << endl;

		if (remCyc)
			applicationShutdown = false;

		++phil;
	}

	if (applicationShutdown)
		appRunning = false;
}

