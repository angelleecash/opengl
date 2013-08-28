CC = gcc
CPP = g++
INCLUDE = -I ~/glew/include -I ~/glfw/include -I ~/glm
CFLAGS = -std=c99 -O4 -Wall -g $(SYSCFLAGS) $(MYCFLAGS) $(INCLUDE)
CPPFLAGS = -Wall -g $(INCLUDE)
LDFLAGS = $(SYSLDFLAGS) $(MYLDFLAGS)
LIBS = -lm -ldl $(SYSLIBS) $(MYLIBS)

SYSCFLAGS =
MYCFLAGS = 

SYSLIBS = 
MYLIBS = ./libglfw.so ./libGLEW.so.1.9

SYSLDFLAGS = 
MYLDFLAGS = -Wl,-R -Wl,/home/chenliang/git_projects/opengl


first_window: first_window.o util.o
	$(CPP) $(CPPFLAGS) $(INCLUDE) -o $@ $(LDFLAGS) first_window.o util.o $(LIBS)  

first_window.o: first_window.cpp


triangle: triangle.o util.o shader_loader.o
	$(CPP) $(CPPFLAGS) -o $@ $(LDFLAGS) triangle.o util.o shader_loader.o $(LIBS)  

triangle.o: triangle.cpp
shader_loader.o: shader_loader.cpp

matrix: matrix.o util.o shader_loader.o
	$(CPP) $(CPPFLAGS) -o $@ $(LDFLAGS) matrix.o util.o shader_loader.o $(LIBS)  

color_cube: color_cube.o util.o shader_loader.o
	$(CPP) $(CPPFLAGS) -o $@ $(LDFLAGS) color_cube.o util.o shader_loader.o $(LIBS)  

texture: texture.o util.o shader_loader.o texture_loader.o
	$(CPP) $(CPPFLAGS) -o $@ $(LDFLAGS) texture.o util.o shader_loader.o texture_loader.o $(LIBS)  

input: input.o util.o shader_loader.o texture_loader.o inputs.o
	$(CPP) $(CPPFLAGS) -o $@ $(LDFLAGS) input.o util.o shader_loader.o texture_loader.o inputs.o $(LIBS)  

model: model.o util.o shader_loader.o texture_loader.o inputs.o model_loader.o
	$(CPP) $(CPPFLAGS) -o $@ $(LDFLAGS) model.o util.o shader_loader.o texture_loader.o inputs.o model_loader.o $(LIBS)  

shader: shader.o util.o shader_loader.o texture_loader.o inputs.o model_loader.o
	$(CPP) $(CPPFLAGS) -o $@ $(LDFLAGS) shader.o util.o shader_loader.o texture_loader.o inputs.o model_loader.o $(LIBS)  

triangle.o: triangle.cpp
shader_loader.o: shader_loader.cpp
color_cube.o:color_cube.cpp
texture.o:texture.cpp
texture_loader.o:texture_loader.cpp
input.o:input.cpp
inputs.o:inputs.cpp
model.o:model.cpp
model_loader.o:model_loader.cpp
shader.o:shader.cpp
util.o: util.cpp


clean:
	rm -f *.o
