CC=gcc
CXX=g++
TCC=tcc
CFLAGS=-Iengine -D_GNU_SOURCE
CXXFLAGS=-Ivendor/imgui -Ivendor/imgui/backends -Ivendor/glfw/include -D_GNU_SOURCE
LIBS=-lGL -lglfw -lX11 -lpthread -ldl

# Objetos de ImGui
IMGUI_OBJS=vendor/imgui/imgui.o vendor/imgui/imgui_draw.o vendor/imgui/imgui_widgets.o vendor/imgui/imgui_tables.o \
           vendor/imgui/backends/imgui_impl_glfw.o vendor/imgui/backends/imgui_impl_opengl3.o

all: bin/tptc bin/engine.o bin/gc.o bin/hub_gui bin/editor_gui

bin/tptc: compiler/transpiler.c | bin
	$(CC) compiler/transpiler.c -o bin/tptc $(CFLAGS)

bin/engine.o: engine/engine.c | bin
	$(CC) -c engine/engine.c -o bin/engine.o $(CFLAGS)

bin/gc.o: engine/gc.c | bin
	$(CC) -c engine/gc.c -o bin/gc.o $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

bin/editor_gui: editor/gui.cpp $(IMGUI_OBJS) | bin
	$(CXX) editor/gui.cpp $(IMGUI_OBJS) -o bin/editor_gui $(CXXFLAGS) $(LIBS)

bin/hub_gui: hub/main.cpp $(IMGUI_OBJS) | bin
	$(CXX) hub/main.cpp $(IMGUI_OBJS) -o bin/hub_gui $(CXXFLAGS) $(LIBS)

bin:
	mkdir -p bin

%.exe: %.tn bin/tptc bin/engine.o bin/gc.o
	./bin/tptc $< $*.c
	$(TCC) $*.c bin/engine.o bin/gc.o -Iengine -o $@

clean:
	rm -rf bin/* *.o *.c app.exe vendor/imgui/*.o vendor/imgui/backends/*.o
