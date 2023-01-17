# Project Name (executable)
PROJECT = libqcmc.so
# Compiler
CC = g++

# Run Options       
COMMANDLINE_OPTIONS = #/dev/ttyS0

# Compiler options during compilation
COMPILE_OPTIONS = -g -std=c++2a -w -shared
# -ansi -pedantic -Wall 

#Header include directories
HEADERS = -I. -I.~/Downloads/Setup/boost_1_80_0/ -I.cflobdd/CFLOBDD -I.cflobdd/CFLOBDD/Solver/uwr/bit_vector/ -I.cflobdd/CFLOBDD/Solver/uwr/assert/ -I.cflobdd/CFLOBDD/Solver/uwr/matrix/ -I.cflobdd/CFLOBDD/Solver/uwr/parsing/
#Libraries for linking
LIBS =

# Dependency options
DEPENDENCY_OPTIONS = -MM

#-- Do not edit below this line --

# Subdirs to search for additional source files
SUBDIRS := cflobdd/CFLOBDD # $(shell ls cflobdd/CFLOBDD/ cflobdd/Solver/uwr/bit_vector cflobdd/CFLOBDD/Solver/uwr/parsing/)
DIRS := ./$(SUBDIRS)
#DIRS += ./cflobdd/CFLOBDD/Solver/uwr/bit_vector + ./cflobdd/CFLOBDD/Solver/uwr/parsing 
#SOURCE_FILES := $(foreach d, $(DIRS), $(wildcard $(d)*.cpp) 
SOURCE_FILES := $(shell ls *.cpp)
# SOURCE_FILES += $(shell find . -maxdepth 1 -mindepth 1 -name \*.cpp -a -not -name main.cpp)
SOURCE_FILES += $(shell ls cflobdd/CFLOBDD/Solver/uwr/bit_vector/*.cpp)
SOURCE_FILES += $(shell ls cflobdd/CFLOBDD/Solver/uwr/parsing/*.cpp)
SOURCE_FILES += $(shell find cflobdd/CFLOBDD -maxdepth 1 -mindepth 1 -name \*.cpp -a -not -name main.cpp)
# [SOURCE_FILES] -= $(shell ls cflobdd/CFLOBDD/main.cpp)
# $(info $(SOURCE_FILES))

# Create an object file of every cpp file
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCE_FILES))

# Dependencies
DEPENDENCIES = $(patsubst %.cpp, %.d, $(SOURCE_FILES))

# Create .d files
%.d: %.cpp
	$(CC) $(DEPENDENCY_OPTIONS) $< -MT "$*.o $*.d" -MF $*.d

# Make $(PROJECT) the default target
all: $(PROJECT)
#$(DEPENDENCIES) 
$(PROJECT): $(OBJECTS)
	$(CC) -shared -o $(PROJECT) $(OBJECTS) $(LIBS)

# Include dependencies (if there are any)
# ifneq "$(strip $(DEPENDENCIES))" ""
#   include $(DEPENDENCIES)
# endif

# Compile every cpp file to an object
# %.cpp 
%.o: %.cpp
	$(CC) -c $(COMPILE_OPTIONS) -o $@ $^ $(HEADERS)

# Build & Run Project
run: $(PROJECT)
	./$(PROJECT) $(COMMANDLINE_OPTIONS)

# Clean & Debug
.PHONY: makefile-debug
makefile-debug:

.PHONY: clean
clean:
	rm -f $(PROJECT) $(OBJECTS)

.PHONY: depclean
depclean:
	rm -f $(DEPENDENCIES)

clean-all: clean depclean