@echo off
set CC=gcc
set CFLAGS=-Iengine -D_GNU_SOURCE
set LIBS=-lgdi32 -lshell32 -luser32 -lopengl32

echo [Windows Build] Compilando Suite TPTC...
mkdir bin 2>nul

%CC% engine/downloader.c -o bin/downloader.exe %CFLAGS%
%CC% compiler/transpiler_pro.c -o bin/tptc_pro.exe %CFLAGS%
%CC% -c engine/engine.c -o bin/engine.o %CFLAGS%
%CC% -c engine/gc.c -o bin/gc.o %CFLAGS%
%CC% -c engine/js_runtime.c -o bin/js_runtime.o %CFLAGS%
%CC% -c engine/hardware_api.c -o bin/hardware_api.o %CFLAGS%

echo [Windows Build] Suite lista. Use 'tptc_pro.exe <proy>' para compilar a nativo.
