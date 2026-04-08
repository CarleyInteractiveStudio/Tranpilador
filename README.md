# TPTC - Transpilador de Código Nativo Pro (v0.6)

Este sistema convierte aplicaciones web (HTML/JS/CSS) en binarios nativos de alto rendimiento integrando un motor real de JavaScript.

## Novedades v0.6
- **Motor JS Integrado:** Soporte para QuickJS, permitiendo ejecutar lógica de JavaScript completa (objetos, arreglos, lógica compleja).
- **Componentes Interactivos:** Nueva etiqueta `<input>` para entrada de datos del usuario.
- **Persistencia de Datos (Storage):** API nativa de almacenamiento clave-valor.
- **Eventos Dinámicos:** Los eventos `onclick` ahora ejecutan scripts de JS reales a través del motor integrado.

## Componentes
- `engine/`: Motor C con soporte para QuickJS y Storage nativo.
- `compiler/`: Transpiler v3 que empaqueta scripts de JS para el motor nativo.
- `android_project/`: Preparación para exportación a móvil.

## Uso
1. Ejecuta `make` para construir el IDE.
2. Crea tu app usando todo el poder de JavaScript.
3. Exporta a nativo y disfruta del rendimiento de C con la flexibilidad de JS.
