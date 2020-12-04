# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yeyusong/DeviceMonitor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yeyusong/DeviceMonitor/build

# Include any dependencies generated for this target.
include src/Module/Utils/INI/CMakeFiles/INI.dir/depend.make

# Include the progress variables for this target.
include src/Module/Utils/INI/CMakeFiles/INI.dir/progress.make

# Include the compile flags for this target's objects.
include src/Module/Utils/INI/CMakeFiles/INI.dir/flags.make

src/Module/Utils/INI/CMakeFiles/INI.dir/inifile.cpp.o: src/Module/Utils/INI/CMakeFiles/INI.dir/flags.make
src/Module/Utils/INI/CMakeFiles/INI.dir/inifile.cpp.o: ../src/Module/Utils/INI/inifile.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yeyusong/DeviceMonitor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/Module/Utils/INI/CMakeFiles/INI.dir/inifile.cpp.o"
	cd /home/yeyusong/DeviceMonitor/build/src/Module/Utils/INI && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/INI.dir/inifile.cpp.o -c /home/yeyusong/DeviceMonitor/src/Module/Utils/INI/inifile.cpp

src/Module/Utils/INI/CMakeFiles/INI.dir/inifile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/INI.dir/inifile.cpp.i"
	cd /home/yeyusong/DeviceMonitor/build/src/Module/Utils/INI && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yeyusong/DeviceMonitor/src/Module/Utils/INI/inifile.cpp > CMakeFiles/INI.dir/inifile.cpp.i

src/Module/Utils/INI/CMakeFiles/INI.dir/inifile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/INI.dir/inifile.cpp.s"
	cd /home/yeyusong/DeviceMonitor/build/src/Module/Utils/INI && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yeyusong/DeviceMonitor/src/Module/Utils/INI/inifile.cpp -o CMakeFiles/INI.dir/inifile.cpp.s

# Object files for target INI
INI_OBJECTS = \
"CMakeFiles/INI.dir/inifile.cpp.o"

# External object files for target INI
INI_EXTERNAL_OBJECTS =

src/Module/Utils/INI/libINI.a: src/Module/Utils/INI/CMakeFiles/INI.dir/inifile.cpp.o
src/Module/Utils/INI/libINI.a: src/Module/Utils/INI/CMakeFiles/INI.dir/build.make
src/Module/Utils/INI/libINI.a: src/Module/Utils/INI/CMakeFiles/INI.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yeyusong/DeviceMonitor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libINI.a"
	cd /home/yeyusong/DeviceMonitor/build/src/Module/Utils/INI && $(CMAKE_COMMAND) -P CMakeFiles/INI.dir/cmake_clean_target.cmake
	cd /home/yeyusong/DeviceMonitor/build/src/Module/Utils/INI && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/INI.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/Module/Utils/INI/CMakeFiles/INI.dir/build: src/Module/Utils/INI/libINI.a

.PHONY : src/Module/Utils/INI/CMakeFiles/INI.dir/build

src/Module/Utils/INI/CMakeFiles/INI.dir/clean:
	cd /home/yeyusong/DeviceMonitor/build/src/Module/Utils/INI && $(CMAKE_COMMAND) -P CMakeFiles/INI.dir/cmake_clean.cmake
.PHONY : src/Module/Utils/INI/CMakeFiles/INI.dir/clean

src/Module/Utils/INI/CMakeFiles/INI.dir/depend:
	cd /home/yeyusong/DeviceMonitor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yeyusong/DeviceMonitor /home/yeyusong/DeviceMonitor/src/Module/Utils/INI /home/yeyusong/DeviceMonitor/build /home/yeyusong/DeviceMonitor/build/src/Module/Utils/INI /home/yeyusong/DeviceMonitor/build/src/Module/Utils/INI/CMakeFiles/INI.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/Module/Utils/INI/CMakeFiles/INI.dir/depend

