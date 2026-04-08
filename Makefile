CC=gcc
TCC=tcc
CFLAGS=-Iengine -D_GNU_SOURCE
all: bin/tptc bin/engine.o bin/gc.o bin/hub_app bin/editor_app
bin/tptc: compiler/transpiler.c | bin
	$(CC) compiler/transpiler.c -o bin/tptc $(CFLAGS)
bin/engine.o: engine/engine.c | bin
	$(CC) -c engine/engine.c -o bin/engine.o $(CFLAGS)
bin/gc.o: engine/gc.c | bin
	$(CC) -c engine/gc.c -o bin/gc.o $(CFLAGS)
bin/hub_app: hub/main.c | bin
	$(CC) hub/main.c -o bin/hub_app $(CFLAGS)
bin/editor_app: editor/main.c | bin
	$(CC) editor/main.c -o bin/editor_app $(CFLAGS)
bin:
	mkdir -p bin
%.exe: %.tn bin/tptc bin/engine.o bin/gc.o
	./bin/tptc $< $*.c
	$(TCC) $*.c bin/engine.o bin/gc.o -Iengine -o $@
clean:
	rm -rf bin/*.o bin/tptc bin/hub_app bin/editor_app *.o *.c app.exe
