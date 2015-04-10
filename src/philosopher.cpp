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

#include "philosopher.h"

/* todo:
 * - States/Signals no couts:
 *   - isHungry
 *   - isEating
 *   - isThinking */

Philosopher::Philosopher() : _leftFork(0), _rightFork(0)
{
}

Philosopher::~Philosopher()
{
}

void Philosopher::setHisForks(ForkToken &leftFork, ForkToken &rightFork)
{
	_leftFork = &leftFork.data();
	_rightFork = &rightFork.data();

	acquireForks.addInput(leftFork);
	acquireForks.addInput(rightFork);

	releaseForks.addOutput(leftFork);
	releaseForks.addOutput(rightFork);
}

void Philosopher::doStuff()
{
}
