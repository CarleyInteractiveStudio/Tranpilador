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

echo [TPTC Build System] Cerrando procesos antiguos para liberar archivos...
taskkill /F /IM hub_gui.exe >nul 2>&1
taskkill /F /IM editor_gui.exe >nul 2>&1
taskkill /F /IM tptc_pro.exe >nul 2>&1
taskkill /F /IM downloader.exe >nul 2>&1
taskkill /F /IM app.exe >nul 2>&1

echo [TPTC Build System] Iniciando compilacion de la Suite...
mkdir bin 2>nul

echo - Compilando Downloader...
%CC% engine/downloader.c -o bin/downloader.exe %CFLAGS%
if %errorlevel% neq 0 goto :error

echo - Compilando Transpilador Pro...
%CC% compiler/transpiler_pro.c -o bin/tptc_pro.exe %CFLAGS%
if %errorlevel% neq 0 goto :error

echo - Compilando Nucleo del Motor...
%CC% -c engine/engine.c -o bin/engine.o %CFLAGS%
if %errorlevel% neq 0 goto :error
%CC% -c engine/gc.c -o bin/gc.o %CFLAGS%
if %errorlevel% neq 0 goto :error
%CC% -c engine/js_runtime.c -o bin/js_runtime.o %CFLAGS%
if %errorlevel% neq 0 goto :error

echo.
echo [TPTC Build System] ¡Exito! Suite compilada correctamente.
echo Si algun archivo no se pudo abrir, asegurese de cerrar todas las ventanas de TPTC antes de compilar.
echo Use 'bin\hub_gui.exe' para comenzar.
pause
exit /b 0

:error
echo.
echo [ERROR] Hubo un fallo durante la compilacion.
echo Esto suele pasar si un programa de la suite aun esta abierto.
echo Cierre todo y vuelva a intentarlo.
pause
exit /b 1
