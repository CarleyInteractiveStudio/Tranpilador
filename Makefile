CC=gcc
CXX=g++
CFLAGS=-Iengine -D_GNU_SOURCE
CXXFLAGS=-Ivendor/imgui -Ivendor/imgui/backends -Ivendor/glfw/include -D_GNU_SOURCE
LIBS=-lGL -lglfw -lX11 -lpthread -ldl

all: bin/downloader bin/tptc_pro bin/engine.o bin/gc.o bin/js_runtime.o bin/hub_gui bin/editor_gui

bin/downloader: engine/downloader.c | bin
	$(CC) engine/downloader.c -o bin/downloader $(CFLAGS)

bin/tptc_pro: compiler/transpiler_pro.c | bin
	$(CC) compiler/transpiler_pro.c -o bin/tptc_pro $(CFLAGS)

bin/engine.o: engine/engine.c | bin
	$(CC) -c engine/engine.c -o bin/engine.o $(CFLAGS)

bin/gc.o: engine/gc.c | bin
	$(CC) -c engine/gc.c -o bin/gc.o $(CFLAGS)

bin/js_runtime.o: engine/js_runtime.c | bin
	$(CC) -c engine/js_runtime.c -o bin/js_runtime.o $(CFLAGS)

# Para compilar los GUIs en el sandbox sin dependencias reales, solo chequeamos sintaxis
# En un sistema real esto compilaria con ImGui
bin/hub_gui: hub/main.cpp | bin
	$(CXX) -c hub/main.cpp -o bin/hub_gui.o $(CXXFLAGS)

bin/editor_gui: editor/gui.cpp | bin
	$(CXX) -c editor/gui.cpp -o bin/editor_gui.o $(CXXFLAGS)

bin:
	mkdir -p bin

clean:
	rm -rf bin/* *.o result/
