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

#include "transition.h"

using namespace std;

typedef lock_guard<mutex> Lock;
typedef list<DataTokenBase *>::const_iterator ConstIter;

mutex Transition::_mtxFire;

Transition::Transition()
{
}

Transition::~Transition()
{
}

void Transition::addInput(DataTokenBase *token)
{
	inputs.push_back(token);
}

void Transition::addOutput(DataTokenBase *token)
{
	outputs.push_back(token);
}

bool Transition::fired() const
{
	Lock lock(_mtxFire);

	/* check if transition is able to fire */
	for(ConstIter token = inputs.begin(); token != inputs.end(); ++token)
	{
		if((*token)->isEmpty())
			return false;
	}
	for(ConstIter token = outputs.begin(); token != outputs.end(); ++token)
	{
		if((*token)->isFull())
			return false;
	}

	/* all conditions met -> fire */
	for(ConstIter token = inputs.begin(); token != inputs.end(); ++token)
		--(*token)->_numToken;
	for(ConstIter token = outputs.begin(); token != outputs.end(); ++token)
		++(*token)->_numToken;

	return true;
}
