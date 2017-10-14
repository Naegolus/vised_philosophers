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

#include <chrono>
#include <ctime>
#include <iostream>
#include "ThreadLoop.h"

using namespace std;

ThreadLoop::ThreadLoop() :
	_p_thread(NULL),
	_timerInterval(20),
	_threadRunning(false),
	_threadIsDone(true)
{
}

ThreadLoop::~ThreadLoop()
{
	if(_p_thread)
		kill();
}

void ThreadLoop::start()
{
	if(_p_thread) {
		cerr << "ThreadLoop: Thread isn't shutdown" << endl;
		return;
	}

	_threadRunning = true;
	_threadIsDone = false;
	_p_thread = new thread(&ThreadLoop::tick, this);
}

void ThreadLoop::start(const uint32_t msec)
{
	if(!msec) {
		cerr << "ThreadLoop: Interval is zero" << endl;
		return;
	}

	if(_p_thread) {
		cerr << "ThreadLoop: Thread isn't shutdown" << endl;
		return;
	}

	_timerInterval = msec;

	start();
}
void ThreadLoop::requestFinish()
{
	_threadRunning = false;
}

bool ThreadLoop::isFinished() const
{
	return _threadIsDone;
}

void ThreadLoop::join()
{
	if(!_p_thread)
		return;

	if(this_thread::get_id() == _p_thread->get_id()) {
		cerr << "ThreadLoop: Design error. join() called by own thread => Deadlock" << endl;
		cerr << "  Must be called by different thread!" << endl;
		return;
	}

	_p_thread->join();
	kill();
}

bool ThreadLoop::join(const uint32_t timeout_ms)
{
	return joinOrShutdown(timeout_ms, false);
}

void ThreadLoop::shutdown()
{
	requestFinish();
	join();
}

bool ThreadLoop::shutdown(const uint32_t timeout_ms)
{
	return joinOrShutdown(timeout_ms, true);
}

bool ThreadLoop::joinOrShutdown(const uint32_t timeout_ms, bool shutdown)
{
	if(!_p_thread)
		return true; /* already shutdown */

	if(this_thread::get_id() == _p_thread->get_id()) {
		cerr << "ThreadLoop: Design error. join() or shutdown() called by own thread => Deadlock" << endl;
		cerr << "  Must be called by different thread!" << endl;
		return false;
	}

	if(shutdown)
		requestFinish();

	if(killedByTimeout(timeout_ms))
		return false;

	_p_thread->join();
	kill();

	return true;
}

bool ThreadLoop::killedByTimeout(uint32_t timeout_ms)
{
	clock_t t;

	t = clock();
	while(!isFinished()) {
		if(((float)(clock() - t) * (TICK_TO_SEC * SEC_TO_MS)) > timeout_ms) {
			cerr << "ThreadLoop: Timeout => Have to kill() thread" << endl;
			kill();
			return true;
		}
	}

	return false;
}

void ThreadLoop::kill()
{
	if(!_p_thread) {
		delete _p_thread;
		_p_thread = 0;
		_threadRunning = false;
		_threadIsDone = true;
	}
}

void ThreadLoop::tick()
{
	chrono::milliseconds interval(_timerInterval);

	while(_threadRunning) {
		this_thread::sleep_for(interval);
		ticked();
	}

	_threadIsDone = true;
}
