# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.3.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.3.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/hw02.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hw02.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw02.dir/flags.make

CMakeFiles/hw02.dir/hw02.c.obj: CMakeFiles/hw02.dir/flags.make
CMakeFiles/hw02.dir/hw02.c.obj: ../hw02.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/hw02.dir/hw02.c.obj"
	C:\PROGRA~1\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\hw02.dir\hw02.c.obj   -c "C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02\hw02.c"

CMakeFiles/hw02.dir/hw02.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw02.dir/hw02.c.i"
	C:\PROGRA~1\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02\hw02.c" > CMakeFiles\hw02.dir\hw02.c.i

CMakeFiles/hw02.dir/hw02.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw02.dir/hw02.c.s"
	C:\PROGRA~1\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02\hw02.c" -o CMakeFiles\hw02.dir\hw02.c.s

# Object files for target hw02
hw02_OBJECTS = \
"CMakeFiles/hw02.dir/hw02.c.obj"

# External object files for target hw02
hw02_EXTERNAL_OBJECTS =

hw02.exe: CMakeFiles/hw02.dir/hw02.c.obj
hw02.exe: CMakeFiles/hw02.dir/build.make
hw02.exe: CMakeFiles/hw02.dir/linklibs.rsp
hw02.exe: CMakeFiles/hw02.dir/objects1.rsp
hw02.exe: CMakeFiles/hw02.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable hw02.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\hw02.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw02.dir/build: hw02.exe

.PHONY : CMakeFiles/hw02.dir/build

CMakeFiles/hw02.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\hw02.dir\cmake_clean.cmake
.PHONY : CMakeFiles/hw02.dir/clean

CMakeFiles/hw02.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02" "C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02" "C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02\cmake-build-debug" "C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02\cmake-build-debug" "C:\Users\Micha\OneDrive - Rensselaer Polytechnic Institute\CSCI2500\hw02\cmake-build-debug\CMakeFiles\hw02.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/hw02.dir/depend
