# Example Makefile script
# Purpose: Demonstrate usage of pkg-config with MRPT libraries
# By: Jose Luis Blanco, 2010.
#
# ========================= *IMPORTANT* ================================
# For this method to work MRPT must be installed in your
# system in a path accesible to pkg-config. To check if pkg-config
# sees MRPT config files, execute:
# pkg-config --list-all | grep mrpt
# If no package appears, MRPT is not installed or something else is wrong.
# ======================================================================
#
 
# Set up basic variables:
CC = g++
CFLAGS = -c -Wall
LDFLAGS = -lpthread

# List of sources:
INCLUDES= 
SOURCES = MongoWrapper.cpp
OBJECTS = $(SOURCES:.cpp=.o)
 
# Name of executable target:
TARGET = mongoWrapper.a
 
# MRPT specific flags:
# Here we invoke "pkg-config" passing it as argument the list of the
# MRPT libraries needed by our program (see available libs
# with "pkg-config --list-all | grep mrpt").
#
CFLAGS += `pkg-config --cflags libmongocxx`
LDFLAGS += `pkg-config --libs libmongocxx`
 
all: $(SOURCES) $(TARGET)
 
$(TARGET): $(OBJECTS)
	ar rvs $@ $(OBJECTS)
 
.cpp.o:
	$(CC) --std=c++11 $(LDFLAGS) $(CFLAGS) $(INCLUDES) $< -o $@
 
clean:
	rm $(OBJECTS) $(TARGET)