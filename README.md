# TPTC - Transpilador de Código Nativo Pro

Este sistema permite convertir aplicaciones web (JS/HTML/CSS) en binarios nativos de alto rendimiento usando C y Tiny C Compiler (TCC).

## Componentes

- **Hub (`bin/hub_app`):** Gestor de proyectos y configuración.
- **Editor (`bin/editor_app`):** Entorno de desarrollo con exportación nativa.
- **Engine (`engine/`):** Motor en C con gestión de memoria (GC) y renderizado.
- **Transpiler (`bin/tptc`):** Convierte archivos `.tn` (Web) a `.c` (Nativo).
- **Compiler (`compiler/`):** TCC integrado para compilación instantánea.

## Uso rápido

1. Ejecuta `make` para construir el sistema.
2. Crea un archivo `app.tn` con tu código HTML/JS.
3. Ejecuta `make app.exe` para generar tu aplicación nativa.
4. Ejecuta `./app.exe` para ver el resultado.
