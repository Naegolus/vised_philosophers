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

#ifndef VISED_PHIL_H_
#define VISED_PHIL_H_

#include <cstdint>
#include "Object.h"
#include "ThreadLoop.h"
#include "Philosopher.h"

class VisedPhil : public Object
{
public:
	VisedPhil();
	virtual ~VisedPhil();

	int exec(int argc, char *argv[]);
	void appCycle();

private:
	void appInit();
	void printStatus();

	bool appRunning;
	uint32_t numPhilosophers;
	Fork *forks;
	Philosopher *philosophers;
	ThreadLoop *threads;

	const uint32_t CHECK_INTERVAL = 20;
};

#endif /* VISED_PHIL_H_ */
