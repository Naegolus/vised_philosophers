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

#include <iostream>

template<typename T>
class DataToken
{
	friend class Transition;

public:
	DataToken() : _p_data(0), _isTaken(false)
	{
	}

	DataToken(T *p_data) : _p_data(p_data), _isTaken(false)
	{
	}

	void bind(T *p_data)
	{
		_p_data = p_data;
	}

	T *data() const /* function data() doesn't change this class, but the data may be changed */
	{
		return _p_data;
	}

	virtual ~DataToken() {}

private:
	bool isTaken() const
	{
		if(!_p_data)
		{
			std::cout << "Error in DataToken::isTaken(): Token requested before data has been bound to token" << std::endl;
			return true; /* we are lying, but no one should use a null pointer */
		}

		return _isTaken;
	}

	T *_p_data;
	bool _isTaken;
};

#endif /* SRC_DATA_TOKEN_H_ */
