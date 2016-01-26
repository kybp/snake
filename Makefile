CPPFLAGS = -std=c++11 -g -Wall -Wextra
INCS = -I/Library/Frameworks/SDL2.framework/Versions/A/Headers/
LDFLAGS = -framework SDL2

snake: main.cc snake.o cell.o
	g++ $(INCS) $(LDFLAGS) $(CPPFLAGS) $^ -o $@

OBJS += snake.o
snake.o: snake.cc snake.hh
	g++ $(INCS) $(CPPFLAGS) -c snake.cc

OBJS += cell.o
cell.o: cell.hh cell.hh
	g++ $(INCS) $(CPPFLAGS) -c cell.cc

clean:
	rm -f snake $(OBJS)
