CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids david

asteroids: asteroids.cpp log.cpp timers.cpp
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids
david: David.cpp log.cpp
    g++ $(CFLAGS) David.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -odavid


clean:
	rm -f asteroids
	rm -f david
	rm -f *.o

