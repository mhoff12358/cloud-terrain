
SDL_INCLUDE = -IC:/MinGW/include
SDL_LIB = -LC:/MinGW/lib -lSDL2main -lSDL2 -lSDL2_image
# GL_INCLUDE = 
GL_LIB = -lglew32 -lopengl32 -lglu32

COMPILER = g++
COMPILER_FLAGS = -Wall -c -g -std=c++11 $(SDL_INCLUDE)
LD_FLAGS = -lmingw32 -mwindows -mconsole -std=c++11 $(GL_LIB) $(SDL_LIB)

EXE = main.exe

all: $(EXE) cloud.exe terrain_writer_test.exe

$(EXE): main.o view_state.o view_drawer.o io_controller.o world_terrain.o game_logic.o shader.o cloud.o perlin_grid.o perlin.o my_math.o
	$(COMPILER) main.o view_state.o view_drawer.o io_controller.o world_terrain.o game_logic.o shader.o perlin_grid.o perlin.o my_math.o cloud.o $(LD_FLAGS) -o $@

main.o: main.cpp main.h
	$(COMPILER) $(COMPILER_FLAGS) main.cpp -o $@

perlin.o: perlin.cpp perlin.h
	$(COMPILER) $(COMPILER_FLAGS) perlin.cpp -o $@

perlin_grid.o: perlin_grid.cpp perlin_grid.h
	$(COMPILER) $(COMPILER_FLAGS) perlin_grid.cpp -o $@

view_state.o: view_state.cpp view_state.h
	$(COMPILER) $(COMPILER_FLAGS) view_state.cpp -o $@

view_drawer.o: view_drawer.cpp view_drawer.h
	$(COMPILER) $(COMPILER_FLAGS) view_drawer.cpp -o $@

io_controller.o: io_controller.cpp io_controller.h
	$(COMPILER) $(COMPILER_FLAGS) io_controller.cpp -o $@

world_terrain.o: world_terrain.cpp world_terrain.h
	$(COMPILER) $(COMPILER_FLAGS) world_terrain.cpp -o $@

game_logic.o: game_logic.cpp game_logic.h
	$(COMPILER) $(COMPILER_FLAGS) game_logic.cpp -o $@

shader.o: shader.cpp shader.h
	$(COMPILER) $(COMPILER_FLAGS) shader.cpp -o $@

my_math.o: my_math.cpp my_math.h
	$(COMPILER) $(COMPILER_FLAGS) my_math.cpp -o $@

cloud.exe: cloud.o cloud_example.cpp
	$(COMPILER) cloud_example.cpp cloud.o $(LD_FLAGS) -o $@

cloud.o: cloud.cpp cloud.h
	$(COMPILER) $(COMPILER_FLAGS) cloud.cpp -O2 -o $@

terrain_writer_test.exe: terrain_writer.o terrain_map.o terrain_writer_test.cpp
	$(COMPILER) terrain_writer_test.cpp terrain_map.o terrain_generator.o terrain_writer.o $(LD_FLAGS) -o $@

terrain_generator.o: terrain_generator.cpp terrain_generator.h terrain_point.h
	$(COMPILER) $(COMPILER_FLAGS) terrain_generator.cpp -o $@

terrain_writer.o: terrain_writer.cpp terrain_writer.h terrain_point.h
	$(COMPILER) $(COMPILER_FLAGS) terrain_writer.cpp -o $@

terrain_map.o: terrain_map.cpp terrain_map.h terrain_point.h terrain_writer.o terrain_generator.o
	$(COMPILER) $(COMPILER_FLAGS) terrain_map.cpp -o $@

clean:
	rm *.o