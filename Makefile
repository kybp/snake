CPPFLAGS = -std=c++11 -g -Wall -Wextra `pkg-config --cflags sdl2`
LDFLAGS = `pkg-config --libs sdl2`

all: $(OBJS) snake

OBJS += snake.o
snake.o: snake.cc snake.hh
	g++ $(CPPFLAGS) -c snake.cc

OBJS += direction.o
direction.o: direction.cc direction.hh
	g++ $(CPPFLAGS) -c direction.cc

OBJS += cell.o
cell.o: cell.cc cell.hh
	g++ $(CPPFLAGS) -c cell.cc

OBJS += level.o
level.o: level.cc level.hh
	g++ $(CPPFLAGS) -c level.cc

snake: main.cc $(OBJS)
	g++ $(LDFLAGS) $(CPPFLAGS) main.cc $(OBJS) -o $@

clean:
	rm -f snake $(OBJS)
