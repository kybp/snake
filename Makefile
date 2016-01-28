CPPFLAGS = -std=c++11 -g -Wall -Wextra
INCS = -I/Library/Frameworks/SDL2.framework/Versions/A/Headers/
LDFLAGS = -framework SDL2

snake: main.cc snake.o direction.o cell.o layout.o
	g++ $(INCS) $(LDFLAGS) $(CPPFLAGS) $^ -o $@

OBJS += snake.o
snake.o: snake.cc snake.hh
	g++ $(INCS) $(CPPFLAGS) -c snake.cc

OBJS += direction.o
direction.o: direction.cc direction.hh
	g++ $(INCS) $(CPPFLAGS) -c direction.cc

OBJS += cell.o
cell.o: cell.cc cell.hh
	g++ $(INCS) $(CPPFLAGS) -c cell.cc

OBJS += layout.o
layout.o: layout.cc layout.hh
	g++ $(INCS) $(CPPFLAGS) -c layout.cc

clean:
	rm -f snake $(OBJS)
