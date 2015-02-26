# ============================================
# CC contains the gcc compiler
# ============================================
CC = g++

# ============================================
# CFLAGS contains the options for the compiler
# ============================================
CFLAGS = -c -Wall

# ============================================
# Paths
# ============================================
DAGGEN_SRC = DagGen/Source/
DYNSCHED_SRC = Source/
DYNSCHED_OBJECTS = DynamicScheduler.o Device.o LeveledDag.o DagGen.o Schedule.o

# ============================================
# make commands
# ============================================

# Default option: compile everything
all: main

# Delete Object files
clean:
	rm -f *.o 

# CPP -> Objects
Schedule.o: $(DYNSCHED_SRC)Schedule.cpp
	$(CC) $(CFLAGS) $(DYNSCHED_SRC)Schedule.cpp

DynamicScheduler.o: $(DYNSCHED_SRC)DynamicScheduler.cpp
	$(CC) $(CFLAGS) $(DYNSCHED_SRC)DynamicScheduler.cpp
	
Device.o: $(DYNSCHED_SRC)Device.cpp
	$(CC) $(CFLAGS) $(DYNSCHED_SRC)Device.cpp
	
LeveledDag.o: $(DYNSCHED_SRC)LeveledDag.cpp
	$(CC) $(CFLAGS) $(DYNSCHED_SRC)LeveledDag.cpp
	
DagGen.o: $(DAGGEN_SRC)DagGen.cpp
	$(CC) $(CFLAGS) $(DAGGEN_SRC)DagGen.cpp
	
# Testing Stub
main: main.o $(DYNSCHED_OBJECTS)
	$(CC) main.o $(DYNSCHED_OBJECTS) -o main.run
	
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
