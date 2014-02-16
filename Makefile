
SDL_INCLUDE = -IC:/MinGW/include
SDL_LIB = -LC:/MinGW/lib -lSDL2main -lSDL2
# GL_INCLUDE = 
GL_LIB = -lopengl32 -lglu32

COMPILER = g++
COMPILER_FLAGS = -Wall -c -g -std=c++11 $(SDL_INCLUDE)
LD_FLAGS = -lmingw32 -mwindows -mconsole -std=c++11 $(GL_LIB) $(SDL_LIB)

EXE = main.exe

all: $(EXE) cloud.exe

$(EXE): main.o view_state.o view_drawer.o io_controller.o
	$(COMPILER) main.o view_state.o view_drawer.o io_controller.o $(LD_FLAGS) -o $@

main.o: main.cpp main.h
	$(COMPILER) $(COMPILER_FLAGS) main.cpp -o $@

view_state.o: view_state.cpp view_state.h
	$(COMPILER) $(COMPILER_FLAGS) view_state.cpp -o $@

view_drawer.o: view_drawer.cpp view_drawer.h
	$(COMPILER) $(COMPILER_FLAGS) view_drawer.cpp -o $@

io_controller.o: io_controller.cpp io_controller.h
	$(COMPILER) $(COMPILER_FLAGS) io_controller.cpp -o $@

cloud.exe: cloud.o
	$(COMPILER) cloud.o $(LD_FLAGS) -o $@

cloud.o: cloud.cpp cloud.h
	$(COMPILER) $(COMPILER_FLAGS) cloud.cpp -o $@

clean:
	rm *.o