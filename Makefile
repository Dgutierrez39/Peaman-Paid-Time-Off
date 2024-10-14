CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

PFILES = #swilkerson.cpp smonungolh.cpp sgonzales.cpp dgutierrez.cpp

all: asteroids Peaman  

asteroids: asteroids.cpp log.cpp timers.cpp
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

Peaman: Peaman.cpp log.cpp timers.cpp $(PFILES)
	g++ $(CFLAGS) $(PFILES) Peaman.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o Peaman

clean:
	rm -f asteroids Peaman
	rm -f *.o

