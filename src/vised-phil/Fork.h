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

#ifndef SRC_FORK_H_
#define SRC_FORK_H_

#include <cstdint>
#include <iostream>
#include <thread>
#include <chrono>
#include "Object.h"

class Fork : public Object
{
public:
	Fork() : dirty(0) {}
	virtual ~Fork()
	{
		if (dirty)
			std::cerr << "Fork: Race condition detected" << std::endl;
	}

	void makeDirty()
	{
		uint32_t _dirty = dirty;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		++_dirty;
		dirty = _dirty;
	}

	void makeClean()
	{
		--dirty;
	}

	bool isDirty() const
	{
		return dirty;
	}

private:
	uint32_t dirty;
};

#endif /* SRC_FORK_H_ */
