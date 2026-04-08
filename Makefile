CC=gcc
TCC=tcc
CFLAGS=-Iengine -D_GNU_SOURCE
all: bin/tptc bin/engine.o bin/gc.o bin/js_runtime.o
bin/tptc: compiler/transpiler.c | bin
	$(CC) compiler/transpiler.c -o bin/tptc $(CFLAGS)
bin/engine.o: engine/engine.c | bin
	$(CC) -c engine/engine.c -o bin/engine.o $(CFLAGS)
bin/gc.o: engine/gc.c | bin
	$(CC) -c engine/gc.c -o bin/gc.o $(CFLAGS)
bin/js_runtime.o: engine/js_runtime.c | bin
	$(CC) -c engine/js_runtime.c -o bin/js_runtime.o $(CFLAGS)
bin:
	mkdir -p bin
%.exe: %.tn bin/tptc bin/engine.o bin/gc.o bin/js_runtime.o
	./bin/tptc $< $*.c
	$(TCC) $*.c bin/engine.o bin/gc.o bin/js_runtime.o -Iengine -o $@
clean:
	rm -rf bin/*.o bin/tptc *.o *.c app.exe
