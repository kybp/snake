CPPFLAGS = -std=c++11 -g -Wall -Wextra `pkg-config --cflags sdl2`
LDFLAGS = `pkg-config --libs sdl2`

snake: main.cc snake.o direction.o cell.o layout.o food.hh
	g++ $(LDFLAGS) $(CPPFLAGS) $^ -o $@

OBJS += snake.o
snake.o: snake.cc snake.hh
	g++ $(CPPFLAGS) -c snake.cc

OBJS += direction.o
direction.o: direction.cc direction.hh
	g++ $(CPPFLAGS) -c direction.cc

OBJS += cell.o
cell.o: cell.cc cell.hh
	g++ $(CPPFLAGS) -c cell.cc

OBJS += layout.o
layout.o: layout.cc layout.hh
	g++ $(CPPFLAGS) -c layout.cc

clean:
	rm -f snake $(OBJS)
