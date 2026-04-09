@echo off
set CC=gcc
set TPTC_CC=bin\tptc_cc.exe
set CFLAGS=-Iengine -D_GNU_SOURCE

echo [TPTC Build System] Verificando entorno...
%CC% --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] No se encontro el compilador GCC. Por favor instale MinGW y añadalo al PATH.
    pause
    exit /b 1
)

echo [TPTC Build System] Iniciando compilacion de la Suite...
mkdir bin 2>nul

%CC% engine/downloader.c -o bin/downloader.exe %CFLAGS%
if %errorlevel% neq 0 goto :error

%CC% compiler/transpiler_pro.c -o bin/tptc_pro.exe %CFLAGS%
if %errorlevel% neq 0 goto :error

%CC% -c engine/engine.c -o bin/engine.o %CFLAGS%
if %errorlevel% neq 0 goto :error

%CC% -c engine/gc.c -o bin/gc.o %CFLAGS%
if %errorlevel% neq 0 goto :error

%CC% -c engine/js_runtime.c -o bin/js_runtime.o %CFLAGS%
if %errorlevel% neq 0 goto :error

echo [TPTC Build System] ¡Exito! Suite compilada correctamente.
echo Use 'bin\hub_gui.exe' para comenzar.
pause
exit /b 0

:error
echo [ERROR] Hubo un fallo durante la compilacion. Revise los mensajes de arriba.
pause
exit /b 1
