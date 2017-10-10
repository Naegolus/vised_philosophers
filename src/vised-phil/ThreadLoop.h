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

#ifndef THREADLOOP_H_
#define THREADLOOP_H_

#include <cstdint>
#include <thread>
#include "Object.h"

class ThreadLoop : public Object
{
public:
	ThreadLoop();
	virtual ~ThreadLoop();

	void start();
	void start(const uint32_t msec);
	void requestFinish();
	bool isFinished() const;
	void join();
	bool join(const uint32_t timeout_ms);
	void shutdown();
	bool shutdown(const uint32_t timeout_ms);
	void kill();

	signal0<> ticked;

private:
	void tick();
	bool joinOrShutdown(const uint32_t timeout_ms, bool shutdown);
	bool killedByTimeout(uint32_t timeout_ms);

	std::thread *_p_thread;
	uint32_t _timerInterval;
	bool _threadRunning;
	bool _threadIsDone;

	const float TICK_TO_SEC = (float)1.0 / CLOCKS_PER_SEC;
	const float SEC_TO_MS = 1000;
};

#endif /* THREADLOOP_H_ */
