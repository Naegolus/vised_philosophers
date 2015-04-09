# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# --                                                                         --
# --  This file is part of the Visual Software Editor project                --
# --  https://github.com/Naegolus/vised                                      --
# --                                                                         --
# --  Author(s):                                                             --
# --      - Helmut, redrocket@gmx.at                                         --
# --                                                                         --
# -----------------------------------------------------------------------------
# --                                                                         --
# --  Copyright (C) 2015 Authors                                             --
# --                                                                         --
# --  This program is free software: you can redistribute it and/or modify   --
# --  it under the terms of the GNU General Public License as published by   --
# --  the Free Software Foundation, either version 3 of the License, or      --
# --  (at your option) any later version.                                    --
# --                                                                         --
# --  This program is distributed in the hope that it will be useful,        --
# --  but WITHOUT ANY WARRANTY; without even the implied warranty of         --
# --  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           --
# --  GNU General Public License for more details.                           --
# --                                                                         --
# --  You should have received a copy of the GNU General Public License      --
# --  along with this program. If not, see <http://www.gnu.org/licenses/>.   --
# --                                                                         --
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

srcDir = ./src
objDir = ./obj

cc = g++
cflags = -O0 -g3 -Wall -fmessage-length=0 -std=c++11 -pthread

ld = g++
ldflags =
libs = pthread
liboptions = $(addprefix -l, $(libs))

# Single threaded application
srcAll = $(wildcard $(srcDir)/*.cpp)

srcSingle = $(filter-out ./src/application_multi.cpp, $(srcAll))
objSingle = $(addprefix $(objDir)/, $(notdir $(srcSingle:%.cpp=%.o)))

# Multi threaded application
srcMulti = $(filter-out ./src/application_single.cpp, $(srcAll))
objMulti = $(addprefix $(objDir)/, $(notdir $(srcMulti:%.cpp=%.o)))

all: obj/single obj/multi

$(objDir)/%.o: $(srcDir)/%.cpp
	$(cc) -c "$<" $(cflags) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@"

# Single threaded application
obj/single: $(objSingle)
	$(ld) -o "$@" $^ $(liboptions)

# Multi threaded application
obj/multi: $(objMulti)
	$(ld) -o "$@" $^ $(liboptions)

clean:
	rm -rf obj/*.o obj/*.d obj/single obj/multi

help:
	@echo
	@echo 'Usage: make [target]'
	@echo 'target:'
	@echo '  all                  default. build everything in this repository'
	@echo '  clean                clean entire repository'
	@echo '  help                 print this message'
	@echo

