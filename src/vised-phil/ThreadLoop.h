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

#ifndef THREADLOOP_H_
#define THREADLOOP_H_

#include <cstdint>
#include <thread>
#include <chrono>
#include <ctime>
#include <iostream>
#include "Object.h"

class ThreadLoop : public Object
{
public:
	ThreadLoop() : _p_thread(NULL),
				_timerInterval(20),
				_threadRunning(false),
				_threadIsDone(true)
	{
	}

	virtual ~ThreadLoop()
	{
		if(_p_thread)
			kill();
	}

	void start()
	{
		if(_p_thread) {
			std::cerr << "ThreadLoop: Thread isn't shutdown" << std::endl;
			return;
		}

		_threadRunning = true;
		_threadIsDone = false;
		_p_thread = new std::thread(&ThreadLoop::tick, this);
	}

	void start(const uint32_t msec)
	{
		if(!msec) {
			std::cerr << "ThreadLoop: Interval is zero" << std::endl;
			return;
		}

		if(_p_thread) {
			std::cerr << "ThreadLoop: Thread isn't shutdown" << std::endl;
			return;
		}

		_timerInterval = msec;

		start();
	}

	void requestFinish()
	{
		_threadRunning = false;
	}

	bool isFinished() const
	{
		return _threadIsDone;
	}

	void join()
	{
		if(!_p_thread)
			return;

		if(std::this_thread::get_id() == _p_thread->get_id()) {
			std::cerr << "ThreadLoop: Design error. join() called by own thread => Deadlock" << std::endl;
			std::cerr << "  Must be called by different thread!" << std::endl;
			return;
		}

		_p_thread->join();
		kill();
	}

	bool join(const uint32_t timeout_ms)
	{
		return joinOrShutdown(timeout_ms, false);
	}

	void shutdown()
	{
		requestFinish();
		join();
	}

	bool shutdown(const uint32_t timeout_ms)
	{
		return joinOrShutdown(timeout_ms, true);
	}

	void kill()
	{
		if(!_p_thread) {
			delete _p_thread;
			_p_thread = 0;
			_threadRunning = false;
			_threadIsDone = true;
		}
	}

	/* signals */
	signal0<> ticked;

private:
	void tick()
	{
		std::chrono::milliseconds interval(_timerInterval);

		while(_threadRunning) {
			std::this_thread::sleep_for(interval);
			ticked();
		}

		_threadIsDone = true;
	}

	bool joinOrShutdown(const uint32_t timeout_ms, bool shutdown)
	{
		if(!_p_thread)
			return true; /* already shutdown */

		if(std::this_thread::get_id() == _p_thread->get_id()) {
			std::cerr << "ThreadLoop: Design error. join() or shutdown() called by own thread => Deadlock" << std::endl;
			std::cerr << "  Must be called by different thread!" << std::endl;
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

	bool killedByTimeout(uint32_t timeout_ms)
	{
		clock_t t;

		t = clock();
		while(!isFinished()) {
			if(((float)(clock() - t) * (TICK_TO_SEC * SEC_TO_MS)) > timeout_ms) {
				std::cerr << "ThreadLoop: Timeout => Have to kill() thread" << std::endl;
				kill();
				return true;
			}
		}

		return false;
	}

	std::thread *_p_thread;
	uint32_t _timerInterval;
	bool _threadRunning;
	bool _threadIsDone;

	const float TICK_TO_SEC = (float)1.0 / CLOCKS_PER_SEC;
	const float SEC_TO_MS = 1000;
};

#endif /* THREADLOOP_H_ */
