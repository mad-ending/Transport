CPP = g++
FLAGS = -std=c++17 -Wall -Wextra

main: main.o udp.o datagram.o validation.o
	$(CPP) $(FLAGS) validation.o datagram.o udp.o main.o -o transport

main.o: main.cpp
	$(CPP) -c $(FLAGS) main.cpp -o main.o

datagram.o: datagram.cpp
	$(CPP) -c $(FLAGS) datagram.cpp -o datagram.o

udp.o: udp.cpp
	$(CPP) -c $(FLAGS) udp.cpp -o udp.o

validation.o: validation.cpp
	$(CPP) -c $(FLAGS) validation.cpp -o validation.o

clean:
	rm -rf *.o

distclean:
	rm -rf *.o transport

all : main
