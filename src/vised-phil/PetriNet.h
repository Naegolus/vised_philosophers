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

#ifndef PETRI_NET_H_
#define PETRI_NET_H_

#include <cstdint>
#include <list>
#include <mutex>

class PetriNet
{
public:
	static PetriNet &instance()
	{
		static PetriNet instance;
		return instance;
	}
	virtual ~PetriNet();

	/* TODO */
	void bindResourcePool() {}

	/*
		false if:
		- pointer to resource is null
		- resource is already added
	*/
	bool addResource(void *resource)
	{
		std::pair<std::map<void *, uint32_t>::iterator, bool> ret;

		ret = globalResources.insert(std::pair<void *, uint32_t>(resource, 1));

		return ret.second;
	}

	/*
		false if:
		- at least one of the resources can't be aquired at the time being
	*/
	bool lockResources(std::list<void *> &transitionResources)
	{
		return false;
	}

	void unlockResources(std::list<void *> &transitionResources)
	{
	}

private:
	static std::mutex resMtx;
	PetriNet();
	PetriNet(PetriNet const &);
	void operator=(PetriNet const &);

	std::map<void *, uint32_t> globalResources;
};

class Transition
{
public:
	Transition();
	virtual ~Transition();

	/*
		false if:
		- pointer to resource is null
		- resource is not found in petri net
		- resource is already bound
	*/
	bool bindResource(void *resource)
	{
		if (!resource)
			return false;

		return false;
	}

	bool lockResources()
	{ return PetriNet::instance().lockResources(resources); }

	void unlockResources()
	{ PetriNet::instance().unlockResources(resources); }

private:
	std::list<void *> resources;
};

#endif /* PETRI_NET_H_ */

