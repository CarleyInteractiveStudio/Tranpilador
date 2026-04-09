ifeq ($(OS),Windows_NT)
    CC=gcc
    CXX=g++
    TPTC_CC=bin/tptc_cc.exe
    LIBS=-lgdi32 -lshell32 -luser32 -lopengl32 -lglfw3 -lpthread -ldl
    EXT=.exe
else
    CC=gcc
    CXX=g++
    TPTC_CC=./bin/tptc_cc
    LIBS=-lGL -lglfw -lX11 -lpthread -ldl
    EXT=
endif

CFLAGS=-Iengine -D_GNU_SOURCE
CXXFLAGS=-Ivendor/imgui -Ivendor/imgui/backends -Ivendor/glfw/include -D_GNU_SOURCE

# Objetos de ImGui (se compilarían si las fuentes estuvieran presentes)
IMGUI_OBJS=vendor/imgui/imgui.o vendor/imgui/imgui_draw.o vendor/imgui/imgui_widgets.o vendor/imgui/imgui_tables.o \
           vendor/imgui/backends/imgui_impl_glfw.o vendor/imgui/backends/imgui_impl_opengl3.o

all: bin/downloader$(EXT) bin/tptc_pro$(EXT) bin/engine.o bin/gc.o bin/js_runtime.o bin/hub_gui$(EXT) bin/editor_gui$(EXT)

bin/downloader$(EXT): engine/downloader.c | bin
	$(CC) engine/downloader.c -o bin/downloader$(EXT) $(CFLAGS)

bin/tptc_pro$(EXT): compiler/transpiler_pro.c | bin
	$(CC) compiler/transpiler_pro.c -o bin/tptc_pro$(EXT) $(CFLAGS)

bin/engine.o: engine/engine.c | bin
	$(CC) -c engine/engine.c -o bin/engine.o $(CFLAGS)

bin/gc.o: engine/gc.c | bin
	$(CC) -c engine/gc.c -o bin/gc.o $(CFLAGS)

bin/js_runtime.o: engine/js_runtime.c | bin
	$(CC) -c engine/js_runtime.c -o bin/js_runtime.o $(CFLAGS)

# Reglas para GUIs
bin/hub_gui$(EXT): hub/main.cpp | bin
	@echo "Compilando HUB GUI..."
	$(CXX) hub/main.cpp -o bin/hub_gui$(EXT) $(CXXFLAGS) $(LIBS) 2>/dev/null || echo "Aviso: No se pudo compilar GUI completo por falta de librerias."

bin/editor_gui$(EXT): editor/gui.cpp | bin
	@echo "Compilando EDITOR GUI..."
	$(CXX) editor/gui.cpp -o bin/editor_gui$(EXT) $(CXXFLAGS) $(LIBS) 2>/dev/null || echo "Aviso: No se pudo compilar GUI completo por falta de librerias."

bin:
	mkdir -p bin

clean:
	rm -rf bin/*.o bin/downloader$(EXT) bin/tptc_pro$(EXT) bin/hub_gui$(EXT) bin/editor_gui$(EXT) *.o *.c result/
