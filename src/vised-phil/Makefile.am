
#  This file is part of the DSP-Crowd project
#  https://www.dsp-crowd.com

#  Author(s):
#      - Johannes Natter, office@dsp-crowd.com

#  Copyright (C) 2017 Authors and www.dsp-crowd.com

#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.

#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with this program. If not, see <http://www.gnu.org/licenses/>.

bin_PROGRAMS = vised-phil

vised_phil_SOURCES = main.cpp VisedPhil.cpp Philosopher.cpp

AM_CPPFLAGS = \
		-I${srcdir}/include \
		-std=c++11

# AM_LDFLAGS could be used
vised_phil_LDFLAGS = -pthread
