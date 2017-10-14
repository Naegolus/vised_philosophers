#!/bin/sh

astyle --suffix=none --style=linux --indent=force-tab=4 --recursive ./*.cpp ./*.c ./*.h
