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

#include "fibonacci.h"

Fibonacci::Fibonacci()
{
}

Fibonacci::~Fibonacci()
{
}

uint32_t Fibonacci::calc(uint32_t num)
{
	if(0 == num or 1 == num)
		return 1;
	else
		return calc(num - 1) + calc(num - 2);

}
