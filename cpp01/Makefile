# Needs to be configured everytime
DEPENDENCIES=
MODS:=server
CPP:=c++
STD :=c++11
CPPFLAGS := -Iinclude
CXXFLAGS := -Wall -Wextra -pedantic-errors -std=$(STD) -O2 -lpthread

all:compile client a

clean:erlclean
	\rm -f *.o client a

client:client.o
	$(CPP) $(CPPFLAGS) $(CXXFLAGS) $< -o client

include erlang.mk
