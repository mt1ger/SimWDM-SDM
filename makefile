CC=g++ 
LDFLAGS=-pthread
CFLAGS=-O3
EXEC=Simulator
#EXEC=test_mf

SOURCES=main.cpp\
        Network.cpp\
		Dijkstra.cpp\
		RoutingTable.cpp\
		TrafficGenerator.cpp\
		Event.cpp\
		EventQueue.cpp\
		ResourceAssignment.cpp\
		RandomVariable.cpp\
		Topology.cpp

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(EXEC)


# OBJS=$(SOURCES:.cpp=.o)
# INCLUDES=-I.
# CLEANFILES=*.o


# all: $(SOURCES) $(EXEC)
# $(EXEC): $(OBJS) 
# 	$(CC) $(INCLUDES) $(LDFLAGS) $(OBJS) -o $@
#
# main.o: main.cpp
# 	$(CC) $(INCLUDES) $(CFLAGS) main.cpp $(WHICH)
# .cpp.o: 
# 	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

# .PHONY: clean
# clean:
# 	rm ${CLEANFILES}

gpp -pthread main.cpp TrafficGenerator.cpp Event.cpp EventQueue.cpp ResourceAssignment.cpp Network.cpp RoutingTable.cpp Topology.cpp Dijkstra.cpp RandomVariable.cpp -o test

