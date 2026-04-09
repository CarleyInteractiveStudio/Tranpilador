CC=gcc
CXX=g++
TPTC_CC=./bin/tptc_cc
CFLAGS=-Iengine -D_GNU_SOURCE
CXXFLAGS=-Ivendor/imgui -Ivendor/imgui/backends -Ivendor/glfw/include -D_GNU_SOURCE
LIBS=-lGL -lglfw -lX11 -lpthread -ldl

all: bin/downloader bin/tptc_pro bin/engine.o bin/gc.o bin/js_runtime.o

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

# REGLA MAESTRA: Usar TPTC_CC para compilar las aplicaciones finales
%.exe: %.c bin/engine.o bin/gc.o bin/js_runtime.o
	$(TPTC_CC) -Icompiler/include -Iengine $< bin/engine.o bin/gc.o bin/js_runtime.o -Lbin -o $@

bin:
	mkdir -p bin

clean:
	rm -rf bin/*.o bin/downloader bin/tptc_pro *.o *.c result/
