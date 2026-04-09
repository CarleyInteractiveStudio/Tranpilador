@echo off
set CC=gcc
set TPTC_CC=bin\tptc_cc.exe
set CFLAGS=-Iengine -D_GNU_SOURCE

echo [Windows Build] Iniciando Suite con TPTC_CC propio...
mkdir bin 2>nul

%CC% engine/downloader.c -o bin/downloader.exe %CFLAGS%
%CC% compiler/transpiler_pro.c -o bin/tptc_pro.exe %CFLAGS%
%CC% -c engine/engine.c -o bin/engine.o %CFLAGS%
%CC% -c engine/gc.c -o bin/gc.o %CFLAGS%
%CC% -c engine/js_runtime.c -o bin/js_runtime.o %CFLAGS%

echo [Windows Build] ¡Exito! Suite lista.
echo Use '%TPTC_CC% app.c engine/engine.o ...' para generar sus ejecutables nativos.
