# TPTC - Transpilador de Código Nativo Pro (v1.0)

Este es el sistema definitivo para convertir aplicaciones web (HTML/JS/CSS) en binarios nativos puros de alto rendimiento, traduciendo JavaScript directamente a código C.

## Hito v1.0: Compilación Real JS-a-C
- **Cero Intérpretes:** JS ya no se interpreta, se **compila** a instrucciones C imperativas.
- **Runtime en C:** Librería de tiempo de ejecución de JS implementada en C (`js_runtime.h/c`) con soporte para tipos dinámicos.
- **Rendimiento Extremo:** Al eliminar el motor de JS en tiempo de ejecución, las apps son más ligeras y rápidas.
- **Librerías del Navegador:** Simulación nativa de `console`, `document` y manipulación del DOM en C.

## Arquitectura
- `engine/`: Motor gráfico y Runtime de JS en C.
- `compiler/`: El compilador de JS-a-C que genera el código fuente final.
- `hub/` & `editor/`: Herramientas visuales de desarrollo.

## Uso
1. `make` para construir las herramientas.
2. `./bin/tptc app.tn app.c` para compilar tu app web a C.
3. Usa TCC para generar el ejecutable final: `tcc app.c ... -o app.exe`.
