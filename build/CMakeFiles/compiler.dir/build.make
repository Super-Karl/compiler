# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wnx/compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wnx/compiler/build

# Include any dependencies generated for this target.
include CMakeFiles/compiler.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/compiler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/compiler.dir/flags.make

CMakeFiles/compiler.dir/src/entry.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/src/entry.cpp.o: ../src/entry.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wnx/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/compiler.dir/src/entry.cpp.o"
	/bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/src/entry.cpp.o -c /home/wnx/compiler/src/entry.cpp

CMakeFiles/compiler.dir/src/entry.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/src/entry.cpp.i"
	/bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wnx/compiler/src/entry.cpp > CMakeFiles/compiler.dir/src/entry.cpp.i

CMakeFiles/compiler.dir/src/entry.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/src/entry.cpp.s"
	/bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wnx/compiler/src/entry.cpp -o CMakeFiles/compiler.dir/src/entry.cpp.s

CMakeFiles/compiler.dir/src/ast/AstNode.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/src/ast/AstNode.cpp.o: ../src/ast/AstNode.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wnx/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/compiler.dir/src/ast/AstNode.cpp.o"
	/bin/clang++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/src/ast/AstNode.cpp.o -c /home/wnx/compiler/src/ast/AstNode.cpp

CMakeFiles/compiler.dir/src/ast/AstNode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/src/ast/AstNode.cpp.i"
	/bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wnx/compiler/src/ast/AstNode.cpp > CMakeFiles/compiler.dir/src/ast/AstNode.cpp.i

CMakeFiles/compiler.dir/src/ast/AstNode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/src/ast/AstNode.cpp.s"
	/bin/clang++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wnx/compiler/src/ast/AstNode.cpp -o CMakeFiles/compiler.dir/src/ast/AstNode.cpp.s

# Object files for target compiler
compiler_OBJECTS = \
"CMakeFiles/compiler.dir/src/entry.cpp.o" \
"CMakeFiles/compiler.dir/src/ast/AstNode.cpp.o"

# External object files for target compiler
compiler_EXTERNAL_OBJECTS =

compiler: CMakeFiles/compiler.dir/src/entry.cpp.o
compiler: CMakeFiles/compiler.dir/src/ast/AstNode.cpp.o
compiler: CMakeFiles/compiler.dir/build.make
compiler: CMakeFiles/compiler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wnx/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable compiler"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/compiler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/compiler.dir/build: compiler

.PHONY : CMakeFiles/compiler.dir/build

CMakeFiles/compiler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/compiler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/compiler.dir/clean

CMakeFiles/compiler.dir/depend:
	cd /home/wnx/compiler/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wnx/compiler /home/wnx/compiler /home/wnx/compiler/build /home/wnx/compiler/build /home/wnx/compiler/build/CMakeFiles/compiler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/compiler.dir/depend

