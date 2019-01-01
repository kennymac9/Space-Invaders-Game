# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
SRCS = Bullet.cpp Ship.cpp Alien.cpp
OBJS = ${SRCS:.cpp=.o}
PROGS = main
DEPS = 
DEPOBJS = Bullet.o Ship.o Alien.o

LIBRARIES = -lsfml-graphics -lsfml-window -lsfml-system

# Default rule. Make all programs.
all: $(PROGS)
.PHONY: all

# Make an individual program.
$(PROGS): % : %.o $(DEPOBJS)
	$(CXX) $(CXXFLAGS) $@.o $(DEPOBJS) -o $@ $(INCLUDE) $(LIBRARIES)

# Make an object file.
Bullet.o: Bullet.cpp Bullet.h $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE) $(LIBRARIES)

Ship.o: Ship.cpp Ship.h $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE) $(LIBRARIES)

Alien.o: Alien.cpp Alien.h $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE) $(LIBRARIES)

main.o: main.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE) $(LIBRARIES)

# Clean Up.
.PHONY: clean
clean:
	@echo "Cleaning up"
	@rm -f $(PROGS) $(OBJS) $(DEPOBJS)

