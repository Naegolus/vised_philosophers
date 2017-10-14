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

#ifndef TRANSITION_H_
#define TRANSITION_H_

#include <mutex>
#include <list>
#include <map>

typedef std::lock_guard<std::mutex> Lock;

class Transition
{
public:
	Transition() {}
	virtual ~Transition() {}

	bool bind(void *resource)
	{
		if (!resource)
			return false;

		allRes.insert(std::pair<void *, uint32_t>(resource, false));

		transitionRes.push_back(resource);
		transitionRes.unique();

		return true;
	}

	/*
		false if:
		- at least one of the resources can't be aquired at the time being
	*/
	bool acquire()
	{
		Lock lock(mtxRes);

		for (std::list<void *>::const_iterator iter = transitionRes.begin(); iter != transitionRes.end(); ++iter)
			if (allRes[*iter] == true) /* Resource already taken */
				return false;

		for (std::list<void *>::iterator iter = transitionRes.begin(); iter != transitionRes.end(); ++iter)
			allRes[*iter] == true;

		return true;
	}

	void release()
	{
		Lock lock(mtxRes);

		for (std::list<void *>::iterator iter = transitionRes.begin(); iter != transitionRes.end(); ++iter)
			allRes[*iter] == false;
	}

private:
	static std::mutex mtxRes;
	static std::map<void *, bool> allRes;

	std::list<void *> transitionRes;
};

#endif /* TRANSITION_H_ */

