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



util.o: util.cpp


clean:
	rm -f *.o
