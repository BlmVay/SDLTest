CC=G:\\mingw\\bin\\gcc.exe
SDLDIR=G:\\mingw
CFLAGS=-I $(SDLDIR)\include\SDL2 -L $(SDLDIR)\lib -lmingw32 -lSDL2main -lSDL2 -O2 -mwindows

main.exe: main.c
	$(CC) main.c $(CFLAGS) -o main.exe
