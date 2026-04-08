# TPTC - Transpilador de Código Nativo Pro (v0.2)

Este sistema convierte aplicaciones web (HTML/JS) en binarios nativos de alto rendimiento.

## Novedades v0.2
- **Interfaz Gráfica (IDE):** Editor visual basado en Dear ImGui para desarrollo interactivo.
- **Motor Gráfico OpenGL:** Soporte para renderizado nativo y acelerado.
- **Transpilador Inteligente:** Ahora soporta eventos `onclick` y una gestión de estados más robusta.
- **Interacción Real:** El código generado ahora soporta lógica interactiva (JS -> C).

## Componentes
- `hub/`: Lanzador de aplicaciones con interfaz gráfica.
- `editor/`: IDE visual con editor de código y botones de exportación.
- `engine/`: Motor nativo en C con GC y soporte OpenGL.
- `compiler/`: Transpilador de `.tn` a `.c`.

## Requisitos
- `gcc`, `g++`, `make`
- Librerías: `glfw`, `OpenGL`

## Uso
1. Ejecuta `make` para construir el IDE.
2. Abre el Hub: `./bin/hub_gui`.
3. Exporta tu app nativa desde el editor.
