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

all: bin/downloader$(EXT) bin/tptc_pro$(EXT) bin/engine.o bin/gc.o bin/js_runtime.o

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

%.exe: %.c bin/engine.o bin/gc.o bin/js_runtime.o
	$(TPTC_CC) -Icompiler/include -Iengine $< bin/engine.o bin/gc.o bin/js_runtime.o -o $@

bin:
	mkdir -p bin

clean:
	rm -rf bin/*.o bin/downloader bin/downloader.exe bin/tptc_pro bin/tptc_pro.exe *.o *.c result/
