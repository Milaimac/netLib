# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/kafier/workspace/NetLib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kafier/workspace/NetLib/build/release-cpp11

# Include any dependencies generated for this target.
include netLib/base/CMakeFiles/netLib_base.dir/depend.make

# Include the progress variables for this target.
include netLib/base/CMakeFiles/netLib_base.dir/progress.make

# Include the compile flags for this target's objects.
include netLib/base/CMakeFiles/netLib_base.dir/flags.make

netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o: ../../netLib/base/AsyncLogging.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/AsyncLogging.cc.o -c /home/kafier/workspace/NetLib/netLib/base/AsyncLogging.cc

netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/AsyncLogging.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/AsyncLogging.cc > CMakeFiles/netLib_base.dir/AsyncLogging.cc.i

netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/AsyncLogging.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/AsyncLogging.cc -o CMakeFiles/netLib_base.dir/AsyncLogging.cc.s

netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o


netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o: ../../netLib/base/CountDownLatch.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/CountDownLatch.cc.o -c /home/kafier/workspace/NetLib/netLib/base/CountDownLatch.cc

netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/CountDownLatch.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/CountDownLatch.cc > CMakeFiles/netLib_base.dir/CountDownLatch.cc.i

netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/CountDownLatch.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/CountDownLatch.cc -o CMakeFiles/netLib_base.dir/CountDownLatch.cc.s

netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o


netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o: ../../netLib/base/CurrentThread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/CurrentThread.cc.o -c /home/kafier/workspace/NetLib/netLib/base/CurrentThread.cc

netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/CurrentThread.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/CurrentThread.cc > CMakeFiles/netLib_base.dir/CurrentThread.cc.i

netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/CurrentThread.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/CurrentThread.cc -o CMakeFiles/netLib_base.dir/CurrentThread.cc.s

netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o


netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o: ../../netLib/base/Date.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/Date.cc.o -c /home/kafier/workspace/NetLib/netLib/base/Date.cc

netLib/base/CMakeFiles/netLib_base.dir/Date.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/Date.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/Date.cc > CMakeFiles/netLib_base.dir/Date.cc.i

netLib/base/CMakeFiles/netLib_base.dir/Date.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/Date.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/Date.cc -o CMakeFiles/netLib_base.dir/Date.cc.s

netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o


netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o: ../../netLib/base/FileUtil.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/FileUtil.cc.o -c /home/kafier/workspace/NetLib/netLib/base/FileUtil.cc

netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/FileUtil.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/FileUtil.cc > CMakeFiles/netLib_base.dir/FileUtil.cc.i

netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/FileUtil.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/FileUtil.cc -o CMakeFiles/netLib_base.dir/FileUtil.cc.s

netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o


netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o: ../../netLib/base/LogFile.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/LogFile.cc.o -c /home/kafier/workspace/NetLib/netLib/base/LogFile.cc

netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/LogFile.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/LogFile.cc > CMakeFiles/netLib_base.dir/LogFile.cc.i

netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/LogFile.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/LogFile.cc -o CMakeFiles/netLib_base.dir/LogFile.cc.s

netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o


netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o: ../../netLib/base/Logging.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/Logging.cc.o -c /home/kafier/workspace/NetLib/netLib/base/Logging.cc

netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/Logging.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/Logging.cc > CMakeFiles/netLib_base.dir/Logging.cc.i

netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/Logging.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/Logging.cc -o CMakeFiles/netLib_base.dir/Logging.cc.s

netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o


netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o: ../../netLib/base/Logstream.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/Logstream.cc.o -c /home/kafier/workspace/NetLib/netLib/base/Logstream.cc

netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/Logstream.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/Logstream.cc > CMakeFiles/netLib_base.dir/Logstream.cc.i

netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/Logstream.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/Logstream.cc -o CMakeFiles/netLib_base.dir/Logstream.cc.s

netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o


netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o: ../../netLib/base/ProcessInfo.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/ProcessInfo.cc.o -c /home/kafier/workspace/NetLib/netLib/base/ProcessInfo.cc

netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/ProcessInfo.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/ProcessInfo.cc > CMakeFiles/netLib_base.dir/ProcessInfo.cc.i

netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/ProcessInfo.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/ProcessInfo.cc -o CMakeFiles/netLib_base.dir/ProcessInfo.cc.s

netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o


netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o: ../../netLib/base/Timestamp.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/Timestamp.cc.o -c /home/kafier/workspace/NetLib/netLib/base/Timestamp.cc

netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/Timestamp.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/Timestamp.cc > CMakeFiles/netLib_base.dir/Timestamp.cc.i

netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/Timestamp.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/Timestamp.cc -o CMakeFiles/netLib_base.dir/Timestamp.cc.s

netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o


netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o: ../../netLib/base/Thread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/Thread.cc.o -c /home/kafier/workspace/NetLib/netLib/base/Thread.cc

netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/Thread.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/Thread.cc > CMakeFiles/netLib_base.dir/Thread.cc.i

netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/Thread.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/Thread.cc -o CMakeFiles/netLib_base.dir/Thread.cc.s

netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o


netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o: netLib/base/CMakeFiles/netLib_base.dir/flags.make
netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o: ../../netLib/base/TimeZone.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netLib_base.dir/TimeZone.cc.o -c /home/kafier/workspace/NetLib/netLib/base/TimeZone.cc

netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netLib_base.dir/TimeZone.cc.i"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kafier/workspace/NetLib/netLib/base/TimeZone.cc > CMakeFiles/netLib_base.dir/TimeZone.cc.i

netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netLib_base.dir/TimeZone.cc.s"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kafier/workspace/NetLib/netLib/base/TimeZone.cc -o CMakeFiles/netLib_base.dir/TimeZone.cc.s

netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o.requires:

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o.requires

netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o.provides: netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o.requires
	$(MAKE) -f netLib/base/CMakeFiles/netLib_base.dir/build.make netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o.provides.build
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o.provides

netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o.provides.build: netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o


# Object files for target netLib_base
netLib_base_OBJECTS = \
"CMakeFiles/netLib_base.dir/AsyncLogging.cc.o" \
"CMakeFiles/netLib_base.dir/CountDownLatch.cc.o" \
"CMakeFiles/netLib_base.dir/CurrentThread.cc.o" \
"CMakeFiles/netLib_base.dir/Date.cc.o" \
"CMakeFiles/netLib_base.dir/FileUtil.cc.o" \
"CMakeFiles/netLib_base.dir/LogFile.cc.o" \
"CMakeFiles/netLib_base.dir/Logging.cc.o" \
"CMakeFiles/netLib_base.dir/Logstream.cc.o" \
"CMakeFiles/netLib_base.dir/ProcessInfo.cc.o" \
"CMakeFiles/netLib_base.dir/Timestamp.cc.o" \
"CMakeFiles/netLib_base.dir/Thread.cc.o" \
"CMakeFiles/netLib_base.dir/TimeZone.cc.o"

# External object files for target netLib_base
netLib_base_EXTERNAL_OBJECTS =

lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/build.make
lib/libnetLib_base.a: netLib/base/CMakeFiles/netLib_base.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kafier/workspace/NetLib/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX static library ../../lib/libnetLib_base.a"
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && $(CMAKE_COMMAND) -P CMakeFiles/netLib_base.dir/cmake_clean_target.cmake
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/netLib_base.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
netLib/base/CMakeFiles/netLib_base.dir/build: lib/libnetLib_base.a

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/build

netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/AsyncLogging.cc.o.requires
netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/CountDownLatch.cc.o.requires
netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/CurrentThread.cc.o.requires
netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/Date.cc.o.requires
netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/FileUtil.cc.o.requires
netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/LogFile.cc.o.requires
netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/Logging.cc.o.requires
netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/Logstream.cc.o.requires
netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/ProcessInfo.cc.o.requires
netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/Timestamp.cc.o.requires
netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/Thread.cc.o.requires
netLib/base/CMakeFiles/netLib_base.dir/requires: netLib/base/CMakeFiles/netLib_base.dir/TimeZone.cc.o.requires

.PHONY : netLib/base/CMakeFiles/netLib_base.dir/requires

netLib/base/CMakeFiles/netLib_base.dir/clean:
	cd /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base && $(CMAKE_COMMAND) -P CMakeFiles/netLib_base.dir/cmake_clean.cmake
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/clean

netLib/base/CMakeFiles/netLib_base.dir/depend:
	cd /home/kafier/workspace/NetLib/build/release-cpp11 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kafier/workspace/NetLib /home/kafier/workspace/NetLib/netLib/base /home/kafier/workspace/NetLib/build/release-cpp11 /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base /home/kafier/workspace/NetLib/build/release-cpp11/netLib/base/CMakeFiles/netLib_base.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : netLib/base/CMakeFiles/netLib_base.dir/depend

