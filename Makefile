CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids Peaman  

asteroids: asteroids.cpp log.cpp timers.cpp
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

Peaman: Peaman.cpp log.cpp timers.cpp
	g++ $(CFLAGS) Peaman.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o Peaman

clean:
	rm -f asteroids Peaman
	rm -f *.o

