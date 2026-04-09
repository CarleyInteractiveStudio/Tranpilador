CC=gcc
CXX=g++
TCC=tcc
CFLAGS=-Iengine -D_GNU_SOURCE
CXXFLAGS=-Ivendor/imgui -Ivendor/imgui/backends -Ivendor/glfw/include -D_GNU_SOURCE
LIBS=-lGL -lglfw -lX11 -lpthread -ldl

# Objetos de ImGui
IMGUI_OBJS=vendor/imgui/imgui.o vendor/imgui/imgui_draw.o vendor/imgui/imgui_widgets.o vendor/imgui/imgui_tables.o \
           vendor/imgui/backends/imgui_impl_glfw.o vendor/imgui/backends/imgui_impl_opengl3.o

all: bin/tptc_pro bin/engine.o bin/gc.o bin/js_runtime.o bin/hub_gui bin/editor_gui

bin/tptc_pro: compiler/transpiler_pro.c | bin
	$(CC) compiler/transpiler_pro.c -o bin/tptc_pro $(CFLAGS)

bin/engine.o: engine/engine.c | bin
	$(CC) -c engine/engine.c -o bin/engine.o $(CFLAGS)

bin/gc.o: engine/gc.c | bin
	$(CC) -c engine/gc.c -o bin/gc.o $(CFLAGS)

bin/js_runtime.o: engine/js_runtime.c | bin
	$(CC) -c engine/js_runtime.c -o bin/js_runtime.o $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

bin/editor_gui: editor/gui.cpp $(IMGUI_OBJS) | bin
	$(CXX) editor/gui.cpp $(IMGUI_OBJS) -o bin/editor_gui $(CXXFLAGS) $(LIBS)

bin/hub_gui: hub/main.cpp $(IMGUI_OBJS) | bin
	$(CXX) hub/main.cpp $(IMGUI_OBJS) -o bin/hub_gui $(CXXFLAGS) $(LIBS)

bin:
	mkdir -p bin

clean:
	rm -rf bin/* *.o *.c result/ vendor/imgui/*.o vendor/imgui/backends/*.o
