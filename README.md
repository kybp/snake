# Snake

This is a Snake game I'm using to practise C++11 and SDL. It requires
SDL2 and SDL2_ttf.

In addition to a regular Snake game, which can be run by starting the
executable without any arguments, or with one integer argument
specifying initial length, it also supports maps which are defined in
a simple text file format. To run a game with maps, pass the
specification files as command line arguments to the program.

This program uses
[GNU FreeFont](https://www.gnu.org/software/freefont/index.html).

## Map files

In map files, the character '#' marks a wall that the snake will die
if it touches.

The letter 'U', 'D', 'L', or 'R' mark the snake's starting location,
with the letter indicating its starting direction. If multiple
starting locations are given, the last one, reading the map left to
right then top to bottom, will be used. If no starting location is
given, the snake will start in the top left corner, moving right.

The letter 'F' marks a spot where food will be generated. Any number
of food cells can be indicated; once they are all eaten, play will
progress to the next level. If no food cells are given on the map, it
will loop infinitely like a regular game, just with the walls and/or
starting position.

Blank spaces mark blank spaces. The window size is constant, but when
a map is loaded there will be as many rows as there are lines in the
map file, and as many columns as the longest line has
characters. Displayed cells will be scaled so that the map is as close
as possible to fitting the window.
