CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

PFILES = dgutierrez.cpp sgonzales.cpp smonungolh.cpp swilkerson.cpp

all: asteroids Peaman  

asteroids: asteroids.cpp log.cpp timers.cpp
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

Peaman: Peaman.cpp log.cpp timers.cpp $(PFILES)
	g++ $(CFLAGS)  Peaman.cpp $(PFILES) log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o Peaman

clean:
	rm -f asteroids Peaman
	rm -f *.o

