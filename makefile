cc = cc
includes = -I/usr/local/include
libs = -L/usr/local/lib
link = -lSDL2 -lSDL2_image -lepoxy -lm -lcglm
flags = -O0 -g -std=c99 -pedantic -Wall
files = main.c visual.c shader.c
exe = main

debug:
	$(cc) $(includes) $(libs) $(link) $(flags) $(files) -o $(exe)
