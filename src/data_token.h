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

#ifndef SRC_DATA_TOKEN_H_
#define SRC_DATA_TOKEN_H_

#include <cstdint>
#include <iostream>

/* Info: Token are only necessary if multiple threads
 *       need access to common resources. The class
 *       Transition manages the access to the resources */

class DataTokenBase
{
	friend class Transition;

public:
	DataTokenBase() : _data(0), _capacity(1), _numToken(1) {}
	DataTokenBase(void *data) : _data(data), _capacity(1), _numToken(1) {}
	DataTokenBase(void *data, uint32_t capacity) : _data(data), _capacity(capacity), _numToken(capacity) {}
	virtual ~DataTokenBase() {}

protected:
	void setData(void *data)
	{
		_data = data;
	}

	void setCapacity(uint32_t capacity)
	{
		_numToken = _capacity = capacity;
	}

	void *voidData() const
	{
		return _data;
	}

private:
	bool isEmpty() const
	{
		if(!_data)
		{
			std::cout << "Error in DataTokenBase::isEmpty(): Token requested before data has been bound to token" << std::endl;
			return true; /* we are lying, but no one should use a null pointer */
		}

		return !_numToken;
	}
	bool isFull() const
	{
		if(!_data)
		{
			std::cout << "Error in DataTokenBase::isFull(): Token released before data has been bound to token" << std::endl;
			return true; /* we are lying, but no one should use a null pointer */
		}

		return _numToken == _capacity;
	}

	void *_data;
	uint32_t _capacity;
	/* this member is changed directly by class Transition */
	uint32_t _numToken;
};

template<typename T>
class DataToken : public DataTokenBase
{
public:
	DataToken() {}
	DataToken(T *data) : DataTokenBase(&data) {}
	DataToken(T *data, uint32_t capacity) : DataTokenBase(data, capacity) {}
	virtual ~DataToken() {}

	void bind(T *data, uint32_t capacity = 1)
	{
		setData(data);
		setCapacity(capacity);
	}

	T *data() const /* function data() doesn't change this class, but the data may be changed */
	{
		return (T *)voidData();
	}
};

#endif /* SRC_DATA_TOKEN_H_ */
